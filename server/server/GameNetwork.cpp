#include "stdafx.h"
#include "GameNetwork.h"


GameNetwork::GameNetwork(boost::asio::io_service& io_service)
: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
m_io_service(io_service),
m_strand(io_service),
m_pClientManager(ClientInfoManager::getInstance()),
m_uThreadCount(boost::thread::hardware_concurrency() - 1)
{
	m_bIsAccepting = false;
	m_pMutex = new boost::mutex();
	m_pLock = new boost::mutex::scoped_lock(m_mutex);
	m_pEventProcess = GameEventProcess::getInstance();
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

		cout << pClient->getObject()->getObjId() << " 適虞戚情闘 羨紗 企奄 " << endl;


		m_acceptor.accept(pClient->Socket());

		std::cout << "適虞戚情闘 羨紗 失因. ClientInfoID: " << pClient->getObject()->getObjId() << std::endl;
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

		//置段 羨紗 獣 鳶填 穿勺
		
		pClient->PostSend(false, initPack.packetSize, (char*)&initPack);

		pClient->PostReceive();

	}*/

	PostAccept();
	m_io_service.run();

	// 食穿備 五乞軒 遣戚 宿馬陥....比斐斐備たたび暗た買溝ばばばばば
}

void GameNetwork::Start()
{
	std::cout << "辞獄 獣拙....." << std::endl;

	PostAccept();
	//m_threadGroup.join_all();
	/*for (int i = 0; i < m_uThreadCount; ++i){
		m_vThread[i]->join();
	}*/

}

void GameNetwork::CloseClientInfo(const unsigned int nClientInfoID)
{
	std::cout << "適虞戚情闘 羨紗 曽戟. 室芝 ID: " << nClientInfoID << std::endl;

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

	//趨朕 什傾球 持失背辞 益楕生稽 左鎧切
	switch (pheader->protocol)
	{
	case PacketType::LOGIN_PACKET:
	{
									 std::cout << "鎧亜 床壱粛陥 :: " << nClientInfoID << std::endl;
									 m_pMutex->lock();
									 std::cout << "鎧亜 彰陥 :: " << nClientInfoID << std::endl;
									 PacketLogin* pPacket = (PacketLogin*)pData;
									 ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);
									 std::cout << "ProcessPacket() - 適虞戚情闘 稽益昔 失因 Id: " << pClient->getObject()->getObjId() << " :: " << nClientInfoID << std::endl;

									 PacketInit initPack;
									 initPack.Init();
									 initPack.id = pPacket->id;

									 initPack.pos_x = pClient->getObject()->m_pPosition->x;
									 initPack.pos_y = pClient->getObject()->m_pPosition->y;
									 initPack.pos_z = pClient->getObject()->m_pPosition->z;

									 initPack.dir_x = pClient->getObject()->m_pDirect->x;
									 initPack.dir_y = pClient->getObject()->m_pDirect->y;
									 initPack.dir_z = pClient->getObject()->m_pDirect->z;

									 //置段 羨紗 獣 鳶填 穿勺
									 GameMap::getInstance()->insertObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID);
									 
									 GameMap::getInstance()->sendObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, false, pPacket->id, (char*)&initPack, PacketType::LOGIN_PACKET_LIST);
									
									 m_pMutex->unlock();
									 std::cout << "鎧亜 鋼崖梅陥 :: " << nClientInfoID << std::endl;
	}
		break;
	case PacketType::MOVE_PACKET:
	{
									PacketMove* pPacket = (PacketMove*)pData;
									ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);
									GameObject* gObj = pClient->getObject();


									unsigned short a_bx;
									unsigned short a_bz;
									a_bx = gObj->m_wBlockX;
									a_bz = gObj->m_wBlockZ;
									gObj->setData(pPacket);

									if (gObj->m_wBlockX != a_bx || gObj->m_wBlockZ != a_bz){
										GameMap::getInstance()->insertObjId(gObj->m_wBlockX, gObj->m_wBlockZ, gObj->getObjId());
										//奄糎 鷺系拭辞 objID薦暗 -> 陥献 鷺系拭 objID脊径
										//爽痕 8鯵 鷺系拭辞亀 薦暗背爽嬢醤 敗
										//歯稽 戚疑廃 爽痕 8鯵 鷺系拭 去舌 硝形醤敗
										if (GameMap::getInstance()->deleteObjId(a_bx, a_bz, gObj->getObjId())){ // 失因梅聖 凶幻
											PacketLogout lPack;
											lPack.Init();
											lPack.id = gObj->getObjId();

											short difX = gObj->m_wBlockX - a_bx;
											short difZ = gObj->m_wBlockZ - a_bz;

											GameMap::getInstance()->sendObjId(a_bx, a_bz, false, gObj->getObjId(), (char*)&lPack, PacketType::LOGOUT_PACKET_LIST);
											PacketInit iPack;
											iPack.Init();
											iPack.pos_x = gObj->m_pPosition->x;
											iPack.pos_y = gObj->m_pPosition->y;
											iPack.pos_z = gObj->m_pPosition->z;
											iPack.dir_x = gObj->m_pDirect->x;
											iPack.dir_y = gObj->m_pDirect->y;
											iPack.dir_z = gObj->m_pDirect->z;
											iPack.iAxis = gObj->m_iAxis;
											iPack.id = gObj->getObjId();
											//焼掘 敗呪 馬檎 拭君害
											GameMap::getInstance()->sendObjId(gObj->m_wBlockX, gObj->m_wBlockZ, false, gObj->getObjId(), (char*)&iPack, PacketType::LOGIN_PACKET_LIST);
											
										}
									}
									std::cout << pPacket->id << " - (" << pPacket->pos_x << ", " << pPacket->pos_y << ", " << pPacket->pos_z << ") \n";

	}
		break;
	}

	return;
}

//析鋼 什傾球拭辞 戚暗 掌
bool GameNetwork::PostAccept()
{
	ClientInfo* pClient = nullptr;
	//while (1){
	//	if ((pClient = m_pClientManager->connectClient()) == nullptr) {
	//		boost::this_thread::sleep(boost::posix_time::seconds(1));
	//		continue;
	//	}

	//	m_bIsAccepting = true;

	//	cout << pClient->getObject()->getObjId() << " 適虞戚情闘 羨紗 企奄 " << endl;


	//	m_acceptor.accept(pClient->Socket());

	//	std::cout << "適虞戚情闘 羨紗 失因. ClientInfoID: " << pClient->getObject()->getObjId() << std::endl;
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

	//	//置段 羨紗 獣 鳶填 穿勺

	//	pClient->PostSend(false, initPack.packetSize, (char*)&initPack);

	//	pClient->PostReceive();

	//}
	
	 while ((pClient = m_pClientManager->connectClient()) == nullptr){
		boost::this_thread::sleep(boost::posix_time::seconds(1));
		continue;
	}

	m_bIsAccepting = true;

	cout << "適虞戚情闘 羨紗 企奄 " << endl;

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

	cout << pClient->getObject()->getObjId() << " 適虞戚情闘 羨紗 企奄 " << endl;

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
		std::cout << "適虞戚情闘 羨紗 失因. ClientInfoID: " << pClientInfo->getObject()->getObjId() << std::endl;
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

		//置段 羨紗 獣 鳶填 穿勺
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
		std::cout << GetCurrentThreadId() << "腰 thread拭辞 適虞戚情闘 羨紗 失因. ClientInfoID: " << pClientInfo->getObject()->getObjId() << std::endl;
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

		//置段 羨紗 獣 鳶填 穿勺
		pClientInfo->PostSend(false, initPack.packetSize, (char*)&initPack);

		pClientInfo->PostReceive();

		PostAccept(threadId);
	}
	else
	{
		std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}