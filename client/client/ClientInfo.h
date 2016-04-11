#pragma once
#include "stdafx.h"

class ChatClient
{
public:
	ChatClient(boost::asio::io_service& io_service)
		: m_IOService(io_service),
		m_Socket(io_service)
	{
		m_bIsLogin = false;
		InitializeCriticalSectionAndSpinCount(&m_lock, 4000);
	}

	~ChatClient()
	{
		EnterCriticalSection(&m_lock);

		while (m_SendDataQueue.empty() == false)
		{
			delete[] m_SendDataQueue.front();
			m_SendDataQueue.pop_front();
		}

		LeaveCriticalSection(&m_lock);

		DeleteCriticalSection(&m_lock);
	}

	bool IsConnecting() { return m_Socket.is_open(); }

	void LoginOK() { m_bIsLogin = true; }
	bool IsLogin() { return m_bIsLogin; }

	void Connect(boost::asio::ip::tcp::endpoint endpoint)
	{
		m_nPacketBufferMark = 0;

		m_Socket.async_connect(endpoint,
			boost::bind(&ChatClient::handle_connect, this,
			boost::asio::placeholders::error)
			);
	}

	void Close()
	{
		if (m_Socket.is_open())
		{
			m_Socket.close();
		}
	}

	void PostSend(const bool bImmediately, const int nSize, char* pData)
	{
		char* pSendData = nullptr;

		EnterCriticalSection(&m_lock);		// 락 시작

		if (bImmediately == false)
		{
			pSendData = new char[nSize];
			memcpy(pSendData, pData, nSize);

			m_SendDataQueue.push_back(pSendData);
		}
		else
		{
			pSendData = pData;
		}

		if (bImmediately || m_SendDataQueue.size() < 2)
		{
			boost::asio::async_write(m_Socket, boost::asio::buffer(pSendData, nSize),
				boost::bind(&ChatClient::handle_write, this,
				boost::asio::placeholders::error,
				boost::asio::placeholders::bytes_transferred)
				);
		}

		LeaveCriticalSection(&m_lock);		// 락 완료
	}



private:

	void PostReceive()
	{
		memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));

		m_Socket.async_read_some
			(
			boost::asio::buffer(m_ReceiveBuffer),
			boost::bind(&ChatClient::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)

			);
	}

	void handle_connect(const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "서버 접속 성공" << std::endl;
			std::cout << "이름을 입력하세요!!" << std::endl;

			PostReceive();
		}
		else
		{
			std::cout << "서버 접속 실패. error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
	}

	void handle_write(const boost::system::error_code& error, size_t bytes_transferred)
	{
		EnterCriticalSection(&m_lock);			// 락 시작

		delete[] m_SendDataQueue.front();
		m_SendDataQueue.pop_front();

		char* pData = nullptr;

		if (m_SendDataQueue.empty() == false)
		{
			pData = m_SendDataQueue.front();
		}

		LeaveCriticalSection(&m_lock);			// 락 완료


		if (pData != nullptr)
		{
			PacketHeader* pHeader = (PacketHeader*)pData;
			PostSend(true, pHeader->packetSize, pData);
		}
	}

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				std::cout << "클라이언트와 연결이 끊어졌습니다" << std::endl;
			}
			else
			{
				std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
			}

			Close();
		}
		else
		{
			memcpy(&m_PacketBuffer[m_nPacketBufferMark], m_ReceiveBuffer.data(), bytes_transferred);

			int nPacketData = m_nPacketBufferMark + bytes_transferred;
			int nReadData = 0;

			while (nPacketData > 0)
			{
				if (nPacketData < sizeof(PacketHeader))
				{
					break;
				}

				PacketHeader* pHeader = (PacketHeader*)&m_PacketBuffer[nReadData];

				if (pHeader->packetSize <= nPacketData)
				{
					ProcessPacket(&m_PacketBuffer[nReadData]);

					nPacketData -= pHeader->packetSize;
					nReadData += pHeader->packetSize;
				}
				else
				{
					break;
				}
			}

			if (nPacketData > 0)
			{
				char TempBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
				memcpy(&TempBuffer[0], &m_PacketBuffer[nReadData], nPacketData);
				memcpy(&m_PacketBuffer[0], &TempBuffer[0], nPacketData);
			}

			m_nPacketBufferMark = nPacketData;


			PostReceive();
		}
	}

	void ProcessPacket(const char*pData)
	{
		PacketHeader* pheader = (PacketHeader*)pData;

		switch (pheader->protocol)
		{
		case PacketType::LOGIN_PACKET :
		{
					   /*PKT_RES_IN* pPacket = (PKT_RES_IN*)pData;

					   LoginOK();

					   std::cout << "클라이언트 로그인 성공 ?: " << pPacket->bIsSuccess << std::endl;*/
		}
			break;
		case PacketType::INIT_PACKET:
		{
		}
			break;
		}
	}



private:
	boost::asio::io_service& m_IOService;
	boost::asio::ip::tcp::socket m_Socket;

	std::array<char, 512> m_ReceiveBuffer;

	int m_nPacketBufferMark;
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];

	CRITICAL_SECTION m_lock;
	std::deque< char* > m_SendDataQueue;

	bool m_bIsLogin;
};
//#define MAX_MESSAGE_LEN 100
//int main()
//{
//	boost::asio::io_service io_service;
//
//	auto endpoint = boost::asio::ip::tcp::endpoint(
//		boost::asio::ip::address::from_string("127.0.0.1"),
//		34100);
//
//	ChatClient Cliet(io_service);
//	Cliet.Connect(endpoint);
//
//	boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_service));
//
//
//	char szInputMessage[MAX_MESSAGE_LEN * 2] = { 0, };
//
//	while (std::cin.getline(szInputMessage, MAX_MESSAGE_LEN))
//	{
//		if (strnlen_s(szInputMessage, MAX_MESSAGE_LEN) == 0)
//		{
//			break;
//		}
//
//		if (Cliet.IsConnecting() == false)
//		{
//			std::cout << "서버와 연결되지 않았습니다" << std::endl;
//			continue;
//		}
//
//		if (Cliet.IsLogin() == false)
//		{
//		}
//		else
//		{
//		}
//	}
//
//	io_service.stop();
//
//	Cliet.Close();
//
//	thread.join();
//
//	std::cout << "클라이언트를 종료해 주세요" << std::endl;
//
//	return 0;
//}