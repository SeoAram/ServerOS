#include "stdafx.h"
#include "GameNetwork.h"


GameNetwork::GameNetwork(boost::asio::io_service& io_service)
: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
m_pClientManager(ClientInfoManager::getInstance())
{
	m_bIsAccepting = false;
	m_pMutex = new boost::mutex();
	m_pLock = new boost::mutex::scoped_lock(m_mutex);
}

GameNetwork::~GameNetwork()
{

}

void GameNetwork::Start()
{
	std::cout << "서버 시작....." << std::endl;

	PostAccept();
}

void GameNetwork::CloseClientInfo(const unsigned int nClientInfoID)
{
	std::cout << "클라이언트 접속 종료. 세션 ID: " << nClientInfoID << std::endl;

	ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);

	PacketLogout pData;
	pData.Init();
	pData.id = nClientInfoID;

	ClientInfo* pClient2;
	for (unsigned int i = 0; i < MAX_CONNECT_CLIENT; ++i){
		pClient2 = m_pClientManager->getClient(i);
		if (pClient2->Socket().is_open() && nClientInfoID != i){
			pClient2->PostSend(false, pData.packetSize, (char*)&pData);
		}
	}

	GameMap::getInstance()->deleteObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID);
	//GameMap::getInstance()->sendObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID, (char*)&pData);
	pClient->closeSocket();
	pClient->getObject()->m_wState = IniData::getInstance()->getData("GAME_OBJECT_LOGOUT");
	//pClient->Socket().close();
	m_pClientManager->returnClient(nClientInfoID);

	if (m_bIsAccepting == false)
	{
		PostAccept();
	}
}

void GameNetwork::ProcessPacket(const unsigned int nClientInfoID, const char*pData)
{
	PacketHeader* pheader = (PacketHeader*)pData;

	//워커 스레드 생성해서 그쪽으로 보내자
	switch (pheader->protocol)
	{
	case PacketType::LOGIN_PACKET:
	{
									 std::cout << "내가 쓰고싶다 :: " << nClientInfoID << std::endl;
									 m_pMutex->lock();
									 std::cout << "내가 쓴다 :: " << nClientInfoID << std::endl;
									 PacketLogin* pPacket = (PacketLogin*)pData;
									 ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);
									 std::cout << "ProcessPacket() - 클라이언트 로그인 성공 Id: " << pClient->getObject()->getObjId() << " :: " << nClientInfoID << std::endl;

									 PacketInit initPack;
									 initPack.Init();
									 initPack.id = pPacket->id;

									 initPack.pos_x = pClient->getObject()->m_pPosition->x;
									 initPack.pos_y = pClient->getObject()->m_pPosition->y;
									 initPack.pos_z = pClient->getObject()->m_pPosition->z;

									 initPack.dir_x = pClient->getObject()->m_pDirect->x;
									 initPack.dir_y = pClient->getObject()->m_pDirect->y;
									 initPack.dir_z = pClient->getObject()->m_pDirect->z;

									 //최초 접속 시 패킷 전송
									 GameMap::getInstance()->insertObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID);
									 
									 PacketInit iPack;
									 iPack.Init();


									 ClientInfo* pClient2;

									 for (unsigned int i = 0; i < MAX_CONNECT_CLIENT; ++i){
										 pClient2 = m_pClientManager->getClient(i);
										 if (pClient2->Socket().is_open() && initPack.id != i){
											 pClient2->PostSend(false, initPack.packetSize, (char*)&initPack);
											 if (pClient2->getObject()->m_wState != IniData::getInstance()->getData("GAME_OBJECT_LOGOUT")){
												 iPack.id = pClient2->getObject()->getObjId();

												 iPack.pos_x = pClient2->getObject()->m_pPosition->x;
												 iPack.pos_y = pClient2->getObject()->m_pPosition->y;
												 iPack.pos_z = pClient2->getObject()->m_pPosition->z;

												 iPack.dir_x = pClient2->getObject()->m_pDirect->x;
												 iPack.dir_y = pClient2->getObject()->m_pDirect->y;
												 iPack.dir_z = pClient2->getObject()->m_pDirect->z;

												 iPack.iAxis = pClient2->getObject()->m_iAxis;
												 pClient->PostSend(false, iPack.packetSize, (char*)&iPack);
											 }

										 }
									 }
									 //GameMap::getInstance()->sendObjId(pClient->getObject()->m_wBlockX, false, pClient->getObject()->m_wBlockZ, nClientInfoID, (char*)&initPack);
									 m_pMutex->unlock();
									 std::cout << "내가 반납했다 :: " << nClientInfoID << std::endl;
	}
		break;
	case PacketType::MOVE_PACKET:
	{
									PacketMove* pPacket = (PacketMove*)pData;
									ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);

									pClient->getObject()->m_pPosition->x = pPacket->pos_x;
									pClient->getObject()->m_pPosition->y = pPacket->pos_y;
									pClient->getObject()->m_pPosition->z = pPacket->pos_z;

									pClient->getObject()->m_pDirect->x = pPacket->dir_x;
									pClient->getObject()->m_pDirect->y = pPacket->dir_y;
									pClient->getObject()->m_pDirect->z = pPacket->dir_z;

									pClient->getObject()->m_iAxis = pPacket->wAxis;

									pClient->getObject()->m_wState = IniData::getInstance()->getData("GAME_OBJECT_ALIVE");

									// std::cout << pPacket->id << " - (" << pPacket->pos_x << ", " << pPacket->pos_y << ", " << pPacket->pos_z << ") \n";

									for (unsigned int i = 0; i < MAX_CONNECT_CLIENT; ++i){
										pClient = m_pClientManager->getClient(i);
										if (pClient->Socket().is_open() && nClientInfoID != i 
											&& pClient->getObject()->m_wState != IniData::getInstance()->getData("GAME_OBJECT_LOGOUT")){
											pClient->PostSend(false, pPacket->packetSize, (char*)pPacket);
										}
									}
	}
		break;
	}

	return;
}

bool GameNetwork::PostAccept()
{
	ClientInfo* pClient = m_pClientManager->connectClient();

	m_bIsAccepting = true;

	cout << "클라이언트 접속 대기 " << endl;
	m_acceptor.async_accept(pClient->Socket(),
		boost::bind(&GameNetwork::handle_accept,
		this,
		pClient,
		boost::asio::placeholders::error)
		);

	return true;
}

void GameNetwork::handle_accept(ClientInfo* pClientInfo, const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << "클라이언트 접속 성공. ClientInfoID: " << pClientInfo->getObject()->getObjId() << std::endl;
		pClientInfo->setSocketOpt(boost::asio::ip::tcp::no_delay(true));
		pClientInfo->Init();

		PacketInit initPack;
		initPack.Init();

		initPack.id = pClientInfo->getObject()->getObjId();

		initPack.pos_x = pClientInfo->getObject()->m_pPosition->x;
		initPack.pos_y = pClientInfo->getObject()->m_pPosition->y;
		initPack.pos_z = pClientInfo->getObject()->m_pPosition->z;

		initPack.dir_x = pClientInfo->getObject()->m_pDirect->x;
		initPack.dir_y = pClientInfo->getObject()->m_pDirect->y;
		initPack.dir_z = pClientInfo->getObject()->m_pDirect->z;

		initPack.iAxis = pClientInfo->getObject()->m_iAxis;

		//최초 접속 시 패킷 전송
		pClientInfo->PostSend(false, initPack.packetSize, (char*)&initPack);
		
		pClientInfo->PostReceive();

		PostAccept();
	}
	else
	{
		std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}