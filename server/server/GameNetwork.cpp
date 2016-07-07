#include "stdafx.h"
#include "GameNetwork.h"


GameNetwork::GameNetwork(boost::asio::io_service& io_service)
: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
m_io_service(io_service),
m_strand(io_service),
m_pClientManager(ClientInfoManager::getInstance()),
m_uThreadCount(boost::thread::hardware_concurrency() -1 )
{
	m_bIsAccepting = false;
	m_pMutex = new boost::mutex();
	m_pLock = new boost::mutex::scoped_lock(m_mutex);
	for (int i = 0; i < m_uThreadCount; ++i){
		//m_threadGroup.create_thread(boost::bind(&GameNetwork::connectThread, this, i));
		//m_threadGroup.create_thread(boost::bind(&boost::asio::io_service::run, &m_io_service));
		//m_vThread.push_back(new boost::thread(&GameNetwork::connectThread, this, i));
		//m_vThread.push_back(new boost::thread(boost::bind(&boost::asio::io_service::run, &m_io_service)));
	}
}

GameNetwork::~GameNetwork()
{
	m_threadGroup.join_all();
}

void GameNetwork::connectThread(const unsigned int threadId){
	std::cout << "Create Thread Id :: "  << threadId << std::endl;
	boost::this_thread::sleep(boost::posix_time::seconds(3));
	
	/*while (1){
		if ((pClient = m_pClientManager->connectClient(threadId, m_uThreadCount)) == nullptr) {
			boost::this_thread::sleep(boost::posix_time::seconds(1)); 
			continue;
		}

		m_bIsAccepting = true;

		cout << pClient->getObject()->getObjId() << " 클라이언트 접속 대기 " << endl;


		m_acceptor.accept(pClient->Socket());

		std::cout << "클라이언트 접속 성공. ClientInfoID: " << pClient->getObject()->getObjId() << std::endl;
		pClient->setSocketOpt(boost::asio::ip::tcp::no_delay(true));
		pClient->Init();

		PacketInit initPack;
		initPack.Init();

		initPack.id = pClient->getObject()->getObjId();

		initPack.pos_x = pClient->getObject()->m_pPosition->x;
		initPack.pos_y = pClient->getObject()->m_pPosition->y;
		initPack.pos_z = pClient->getObject()->m_pPosition->z;

		initPack.dir_x = pClient->getObject()->m_pDirect->x;
		initPack.dir_y = pClient->getObject()->m_pDirect->y;
		initPack.dir_z = pClient->getObject()->m_pDirect->z;

		initPack.iAxis = pClient->getObject()->m_iAxis;

		//최초 접속 시 패킷 전송
		
		pClient->PostSend(false, initPack.packetSize, (char*)&initPack);

		pClient->PostReceive();

	}*/

	PostAccept();
	m_io_service.run();

	// 여전히 메모리 릭이 심하다....흑흐흐히ㅏㅏㅣ거ㅏ허뮤ㅠㅠㅠㅠㅠ
}

void GameNetwork::Start()
{
	std::cout << "서버 시작....." << std::endl;

	PostAccept();
	//m_threadGroup.join_all();
	/*for (int i = 0; i < m_uThreadCount; ++i){
		m_vThread[i]->join();
	}*/

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
	pClient->closeSocket();
	pClient->getObject()->m_wState = IniData::getInstance()->getData("GAME_OBJECT_LOGOUT");
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
									 //GameMap::getInstance()->sendObjId(initPack.pos_x / BLOCK_COUNT, initPack.pos_z / BLOCK_COUNT, pPacket->id, false, (char*)&initPack);
									 std::vector<int>& v = GameMap::getInstance()->getObjIdList(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, pPacket->id);
									 
									 PacketInit iPack;
									 iPack.Init();


									 ClientInfo* pClient2;

									 for (unsigned int i = 0; i < v.size() ; ++i){
										 pClient2 = m_pClientManager->getClient(v[i]);
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

//일반 스레드에서 이거 씀
bool GameNetwork::PostAccept()
{
	ClientInfo* pClient = nullptr;
	//while (1){
	//	if ((pClient = m_pClientManager->connectClient()) == nullptr) {
	//		boost::this_thread::sleep(boost::posix_time::seconds(1));
	//		continue;
	//	}

	//	m_bIsAccepting = true;

	//	cout << pClient->getObject()->getObjId() << " 클라이언트 접속 대기 " << endl;


	//	m_acceptor.accept(pClient->Socket());

	//	std::cout << "클라이언트 접속 성공. ClientInfoID: " << pClient->getObject()->getObjId() << std::endl;
	//	pClient->setSocketOpt(boost::asio::ip::tcp::no_delay(true));
	//	pClient->Init();

	//	PacketInit initPack;
	//	initPack.Init();

	//	initPack.id = pClient->getObject()->getObjId();

	//	initPack.pos_x = pClient->getObject()->m_pPosition->x;
	//	initPack.pos_y = pClient->getObject()->m_pPosition->y;
	//	initPack.pos_z = pClient->getObject()->m_pPosition->z;

	//	initPack.dir_x = pClient->getObject()->m_pDirect->x;
	//	initPack.dir_y = pClient->getObject()->m_pDirect->y;
	//	initPack.dir_z = pClient->getObject()->m_pDirect->z;

	//	initPack.iAxis = pClient->getObject()->m_iAxis;

	//	//최초 접속 시 패킷 전송

	//	pClient->PostSend(false, initPack.packetSize, (char*)&initPack);

	//	pClient->PostReceive();

	//}
	
	 while ((pClient = m_pClientManager->connectClient()) == nullptr){
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		continue;
	}

	m_bIsAccepting = true;

	cout << "클라이언트 접속 대기 " << endl;

	m_acceptor.async_accept(
		pClient->Socket(),
		m_strand.wrap(
			boost::bind(&GameNetwork::handle_accept,
				this,
				pClient,
				boost::asio::placeholders::error)
			)
		);
	return true;
}

bool GameNetwork::PostAccept(const unsigned int threadId)
{
	ClientInfo* pClient = nullptr;
	while ((pClient = m_pClientManager->connectClient()) == nullptr){
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		continue;
	}
	/*if (pClient->getObject()->getObjId() % m_uThreadCount != threadId){
		m_pClientManager->returnClient(pClient->getObject()->getObjId());
		pClient = nullptr;
		while ((pClient = m_pClientManager->connectClient()) == nullptr){
			boost::this_thread::sleep(boost::posix_time::seconds(1));
			continue;
		}
	}*/

	m_bIsAccepting = true;

	cout << pClient->getObject()->getObjId() << " 클라이언트 접속 대기 " << endl;

	m_acceptor.async_accept(pClient->Socket(),
		m_strand.wrap(
			boost::bind(&GameNetwork::handle_accept,
				this,
				pClient,
				threadId,
				boost::asio::placeholders::error)
			)
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
		pClientInfo->getObject()->m_wState = IniData::getInstance()->getData("GAME_OBJECT_LOGIN");

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


void GameNetwork::handle_accept(ClientInfo* pClientInfo, const unsigned int threadId, const boost::system::error_code& error)
{
	if (!error)
	{
		std::cout << GetCurrentThreadId() << "번 thread에서 클라이언트 접속 성공. ClientInfoID: " << pClientInfo->getObject()->getObjId() << std::endl;
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

		PostAccept(threadId);
	}
	else
	{
		std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}