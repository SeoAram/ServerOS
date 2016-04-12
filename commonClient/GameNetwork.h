#pragma once

#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

#include "../common/iniRead.h"
#include "../common/protocol.h"
#include "../common/PointVector3D.h"

#include "GameObject.h"

class GameNetwork
{
	
public:
	void Start()
	{
		std::cout << "Ŭ���̾�Ʈ ����....." << std::endl;

	}

	void ProcessPacket(const int nClientInfoID, const char*pData)
	{
		PacketHeader* pheader = (PacketHeader*)pData;

		//��Ŀ ������ �����ؼ� �������� ������
		switch (pheader->protocol)
		{
		case PacketType::INIT_PACKET:
		{
										PacketInit* pPacket = (PacketInit*)pData;
										 //ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);
										 //std::cout << "Ŭ���̾�Ʈ �α��� ���� Id: " << pClient->getObject()->getObjId() << std::endl;

										 //PacketInit initPack;
										 //initPack.Init();
										 //initPack.id = nClientInfoID;

										 //initPack.pos_x = pClient->getObject()->m_pPosition->x;
										 //initPack.pos_y = pClient->getObject()->m_pPosition->y;
										 //initPack.pos_z = pClient->getObject()->m_pPosition->z;

										 //initPack.dir_x = pClient->getObject()->m_pDirect->x;
										 //initPack.dir_y = pClient->getObject()->m_pDirect->y;
										 //initPack.dir_z = pClient->getObject()->m_pDirect->z;
		}
			break;
		case PacketType::MOVE_PACKET:
		{
										PacketMove* pPacket = (PacketMove*)pData;
										/*ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);*/
		}
			break;
		}

		return;
	}

	static GameNetwork* getInstance(boost::asio::io_service& io_service){
		static GameNetwork instance(io_service);
		return &instance;
	}

private:
	GameNetwork(boost::asio::io_service& io_service)
		/*: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
		m_pClientManager(ClientInfoManager::getInstance())*/
	{
	}

	~GameNetwork(){}

	//boost::asio::ip::tcp::io_service m_service;
	//ClientInfoManager* m_pClientManager;
};

