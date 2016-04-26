#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <boost/asio.hpp>
#include <boost/bind.hpp>

#include "../common/iniRead.h"
#include "../common/protocol.h"
#include "../common/PointVector3D.h"

#include "GameObject.h"

class ClientInfo
{
public:
	ClientInfo(boost::asio::io_service& io_service)
		: m_Socket(io_service)
	{
		m_pObject = new GameObject(-1);
	}
	ClientInfo(unsigned int i, boost::asio::io_service& io_service)
		: m_Socket(io_service)
	{
		m_pObject = new GameObject(i);
	}

	ClientInfo(unsigned int i, boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint& endpoint)
		: m_Socket(io_service)
	{
		m_pObject = new GameObject(i);
		connectClient(endpoint);
	}

	ClientInfo(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint& endpoint)
		: m_Socket(io_service)
	{
		m_pObject = new GameObject(-1);
		connectClient(endpoint);
	}
	~ClientInfo()
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
			boost::bind(&ClientInfo::handle_receive, this,
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
			boost::bind(&ClientInfo::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
			);
	}

	GameObject* getObject(){ return m_pObject; }

	void connectClient(boost::asio::ip::tcp::endpoint& endpoint){
		m_nPacketBufferMark = 0;
		m_Socket.async_connect(endpoint, boost::bind(&ClientInfo::handle_connect, this, boost::asio::placeholders::error));
	}

private:
	void handle_connect(const boost::system::error_code& error){
		if (error){
			std::cout << "Connect Failed :: " << error.message() << std::endl;
		}
		else{
			std::cout << "Connect Success! " << std::endl;

			PostReceive();
		}
	}
	void ProcessPacket(const unsigned int nClientInfoID, const char*pData)
	{
		std::cout << m_pObject->getObjId() << " Recv " << nClientInfoID << endl;
		PacketHeader* pheader = (PacketHeader*)pData;

		switch (pheader->protocol)
		{
		case PacketType::INIT_PACKET:
		{
										PacketInit* pPacket = (PacketInit*)pData;
										if (m_pObject->m_cObjState == IniData::getInstance()->getData("GAME_OBJECT_STAT")){

											m_pObject->setObjId(pPacket->id);
											m_pObject->m_pvPos->setXYZ(pPacket->pos_x, pPacket->pos_y, pPacket->pos_z);
											m_pObject->m_pvDir->setXYZ(pPacket->dir_x, pPacket->dir_y, pPacket->dir_z);

											PacketMove movePack;
											movePack.Init();
											movePack.id = pPacket->id;

											movePack.pos_x = getObject()->m_pvPos->x;
											movePack.pos_y = getObject()->m_pvPos->y;
											movePack.pos_z = getObject()->m_pvPos->z;

											movePack.dir_x = getObject()->m_pvDir->x;
											movePack.dir_y = getObject()->m_pvDir->y;
											movePack.dir_z = getObject()->m_pvDir->z;

											movePack.wAxis = m_pObject->getAxis();

											m_pObject->m_cObjState = IniData::getInstance()->getData("GAME_OBJECT_ALIVE");

											PacketLogin lPack;
											lPack.Init();
											lPack.id = pPacket->id;
											PostSend(false, lPack.packetSize, (char*)&lPack);
										}

		}
			break;
		case PacketType::MOVE_PACKET:
		{
										PacketMove* pPacket = (PacketMove*)pData;
										if (pPacket->id == m_pObject->getObjId())
											PostSend(false, pPacket->packetSize, (char*)pPacket);
		}
			break;
		}

		return;
	}


	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{
		delete[] m_SendDataQueue.front();	//여기가 문제라는 것 가ㅌ음
		m_SendDataQueue.pop_front();

		if (m_SendDataQueue.empty() == false)
		{
			char* pData = m_SendDataQueue.front();

			PacketHeader* pHeader = (PacketHeader*)pData;

			PostSend(true, pHeader->packetSize, pData);
		}
	}
	//접속 패킷을 받아봅시다
	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				std::cout << "서버와 연결이 끊어졌습니다" << std::endl;
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

	GameObject* m_pObject;	//object클래스(객체 이동 관리)
	
	std::array<char, MAX_RECEIVE_BUFFER_LEN> m_ReceiveBuffer; // 받는패킷 버퍼

	int m_nPacketBufferMark;	//수신 패킷의 위치
	char m_PacketBuffer[MAX_RECEIVE_BUFFER_LEN * 2];	//...??

	std::deque< char* > m_SendDataQueue;
};

