#pragma once
#include "stdafx.h"

class ClientInfo2D{
public:
	ClientInfo2D(boost::asio::io_service& io_service)
		: m_Socket(io_service)
	{
		m_pObject = new GameObject(-1);
	}
	ClientInfo2D(unsigned int i, boost::asio::io_service& io_service)
		: m_Socket(io_service)
	{
		m_pObject = new GameObject(i);
	}

	ClientInfo2D(unsigned int i, boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint& endpoint)
		: m_Socket(io_service)
	{
		m_pObject = new GameObject(i);
		connectClient(endpoint);
	}

	ClientInfo2D(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint& endpoint)
		: m_Socket(io_service)
	{
		m_pObject = new GameObject(-1);
		connectClient(endpoint);
	}
	~ClientInfo2D()
	{
		while (m_SendDataQueue.empty() == false)
		{
			delete[] m_SendDataQueue.front();
			m_SendDataQueue.pop_front();
		}
	}

	boost::asio::ip::tcp::socket& Socket(){
		return m_Socket;
	}

	void PostReceive()
	{
		m_Socket.async_read_some
			(
			boost::asio::buffer(m_ReceiveBuffer),
			boost::bind(&ClientInfo2D::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
			);
	}
	void PostSend(const bool bImmediately, const int nSize, char* pData){
		char* pSendData = nullptr;

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

		if (bImmediately == false && m_SendDataQueue.size() > 1)
		{
			return;
		}

		boost::asio::async_write(m_Socket, boost::asio::buffer(pSendData, nSize),
			boost::bind(&ClientInfo2D::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
			);
	}

	GameObject* getObject(){ return m_pObject; }

	void connectClient(boost::asio::ip::tcp::endpoint& endpoint){
		m_nPacketBufferMark = 0;
		m_Socket.async_connect(endpoint, boost::bind(&ClientInfo2D::handle_connect, this, boost::asio::placeholders::error));
	}

private:
	void handle_connect(const boost::system::error_code& error){
		if (error){
			std::cout << "Connect Failed :: " << error.message() << std::endl;
		}
		else{
			std::cout << "Connect Success! " << std::endl;

			PacketLogin pData;
			pData.Init();
			PostSend(false, pData.packetSize, (char*)&pData);
			PostReceive();
		}
	}
	void ProcessPacket(const int nClientInfo2DID, const char*pData)
	{
		PacketHeader* pheader = (PacketHeader*)pData;

		switch (pheader->protocol)
		{
		case PacketType::INIT_PACKET:
		{
										PacketInit* pPacket = (PacketInit*)pData;
										
										if (m_pObject->getObjId() == -1){

											std::cout << "Recv Client :: " << pPacket->id << std::endl;
											m_pObject->setObjId(pPacket->id);
											m_pObject->initData(*pPacket);

										}
										else{
											std::cout << "Recv New Client :: " << pPacket->id << std::endl;
											GameObjectManager::getInstance()->setObject(pPacket->id, *pPacket);
										}
										
		}
			break;
		case PacketType::MOVE_PACKET:
		{
										PacketMove* pPacket = (PacketMove*)pData;
										if (m_pObject->getObjId() == pPacket->id){

										}
										else{

										}
		}
			break;
		}

		return;
	}


	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{
		delete[] m_SendDataQueue.front();	//���Ⱑ ������� �� ������
		m_SendDataQueue.pop_front();

		if (m_SendDataQueue.empty() == false)
		{
			char* pData = m_SendDataQueue.front();

			PacketHeader* pHeader = (PacketHeader*)pData;

			PostSend(true, pHeader->packetSize, pData);
		}
	}
	//���� ��Ŷ�� �޾ƺ��ô�
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				std::cout << "������ ������ ���������ϴ�" << std::endl;
			}
			else
			{
				std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
			}
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
					ProcessPacket(pHeader->id, &m_PacketBuffer[nReadData]);
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
	boost::asio::ip::tcp::socket m_Socket;

	GameObject* m_pObject;	//objectŬ����(��ü �̵� ����)
	
	std::array<char, MAX_RECEIVE_BUFFER_LEN> m_ReceiveBuffer; // �޴���Ŷ ����

	int m_nPacketBufferMark;	//���� ��Ŷ�� ��ġ
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];	//...??

	std::deque< char* > m_SendDataQueue;

};