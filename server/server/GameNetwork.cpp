#include "stdafx.h"
#include "GameNetwork.h"


GameNetwork::GameNetwork(boost::asio::io_service& io_service)
: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
m_pClientManager(ClientInfoManager::getInstance())
{
	m_bIsAccepting = false;
	m_pLock = new boost::mutex::scoped_lock(m_mutex);
	/*for (int i = 0; i < WORKED_THREAD; ++i){
	m_tGroup.create_thread(boost::bind(&boost::asio::io_service::run, &io_service));
	cout << "Workerthread Create" << endl;
	}*/
}

GameNetwork::~GameNetwork()
{

}

void GameNetwork::Start()
{
	std::cout << "서버 시작....." << std::endl;

	PostAccept();
}

void GameNetwork::CloseClientInfo(const int nClientInfoID)
{
	std::cout << "클라이언트 접속 종료. 세션 ID: " << nClientInfoID << std::endl;

	ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);

	PacketLogout pData;
	pData.Init();
	pData.id = nClientInfoID;
	GameMap::getInstance()->deleteObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID);
	//GameMap::getInstance()->sendObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID, (char*)&pData);

	pClient->Socket().close();
	m_pClientManager->returnClient(nClientInfoID);

	if (m_bIsAccepting == false)
	{
		PostAccept();
	}
}

void GameNetwork::ProcessPacket(const int nClientInfoID, const char*pData)
{
	PacketHeader* pheader = (PacketHeader*)pData;

	std::cout << nClientInfoID << ":: 냥냥\t";
	//워커 스레드 생성해서 그쪽으로 보내자
	switch (pheader->protocol)
	{
	case PacketType::LOGIN_PACKET:
	{
									 PacketLogin* pPacket = (PacketLogin*)pData;
									 ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);
									 std::cout << "클라이언트 로그인 성공 Id: " << pClient->getObject()->getObjId() << std::endl;

									 PacketInit initPack;
									 initPack.Init();
									 initPack.id = nClientInfoID;

									 initPack.pos_x = pClient->getObject()->m_pPosition->x;
									 initPack.pos_y = pClient->getObject()->m_pPosition->y;
									 initPack.pos_z = pClient->getObject()->m_pPosition->z;

									 initPack.dir_x = pClient->getObject()->m_pDirect->x;
									 initPack.dir_y = pClient->getObject()->m_pDirect->y;
									 initPack.dir_z = pClient->getObject()->m_pDirect->z;

									 //최초 접속 시 패킷 전송
									 pClient->PostSend(false, initPack.packetSize, (char*)&initPack);
									 for (int i = 0; i < MAX_CONNECT_CLIENT; ++i){
										 pClient = m_pClientManager->getClient(i);
										 if(pClient->Socket().is_open() && nClientInfoID != i)
											pClient->PostSend(false, initPack.packetSize, (char*)&initPack);
									 }

									 //GameMap::getInstance()->insertObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID);
									 //GameMap::getInstance()->sendObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID, (char*)&initPack);
	}
		break;
	case PacketType::MOVE_PACKET:
	{
									PacketMove* pPacket = (PacketMove*)pData;
									ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);

									for (int i = 0; i < MAX_CONNECT_CLIENT; ++i){
										pClient = m_pClientManager->getClient(i);
										if (pClient->Socket().is_open() && nClientInfoID != i){
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
		pClientInfo->PostReceive();

		PostAccept();
	}
	else
	{
		std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}