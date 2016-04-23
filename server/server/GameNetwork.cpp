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
	//GameMap::getInstance()->deleteObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID);
	//if (GameMap::getInstance()->deleteObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID)){
		//GameMap::getInstance()->sendObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID, (char*)&pData);
	//}

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

	//워커 스레드 생성해서 그쪽으로 보내자
	switch (pheader->protocol)
	{
	case PacketType::LOGIN_PACKET:
	{
		PacketLogin* pPacket = (PacketLogin*)pData;
		ClientInfo* pClient = m_pClientManager->getClient(nClientInfoID);
		std::cout << "클라이언트 로그인 성공 Id: " << pClient->getObject()->getObjId() << std::endl;

		//GameMap::getInstance()->insertObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID);

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
		
		//GameMap::getInstance()->sendObjId(pClient->getObject()->m_wBlockX, pClient->getObject()->m_wBlockZ, nClientInfoID, (char*)&initPack);
	}
		break;
	case PacketType::MOVE_PACKET:
	{
		PacketMove* pPacket = (PacketMove*)pData;
		ClientInfo* pClient = m_pClientManager->getClient(pPacket->id);
		GameObject* pObject = pClient->getObject();
		
		GameMap* pGameMap = GameMap::getInstance();

		pObject->m_pPosition->x = pPacket->pos_x;
		pObject->m_pPosition->y = pPacket->pos_y;
		pObject->m_pPosition->z = pPacket->pos_z;
		pObject->m_pDirect->x = pPacket->dir_x;
		pObject->m_pDirect->y = pPacket->dir_y;
		pObject->m_pDirect->z = pPacket->dir_z;

		int bx = ((int)pObject->m_pPosition->x / pGameMap->getBlockW());
		int bz = ((int)pObject->m_pPosition->z / pGameMap->getBlockH());

		if (pObject->m_wBlockX != bx || pObject->m_wBlockZ != bz){
			//기존 블록에서 objID제거 -> 다른 블록에 objID입력
			//주변 8개 블록에서도 제거해주어야 함
			//새로 이동한 주변 8개 블록에 등장 알려야함
			if (pGameMap->deleteObjId(pObject->m_wBlockX, pObject->m_wBlockZ, pObject->getObjId())){ // 성공했을 때만
				pGameMap->insertObjId(bx, bz, pObject->getObjId());
				pObject->m_wBlockX = bx;
				pObject->m_wBlockZ = bz;
			}
		}
	}
		break;
	}

	return;
}

bool GameNetwork::PostAccept()
{
	ClientInfo* pClient = nullptr;
	while((pClient = m_pClientManager->connectClient()) == nullptr);

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

		pClientInfo->Init();
		pClientInfo->PostReceive();

		PostAccept();
	}
	else
	{
		std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}