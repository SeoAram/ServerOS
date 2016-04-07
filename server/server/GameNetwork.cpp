#include "stdafx.h"
#include "GameNetwork.h"


GameNetwork::GameNetwork(boost::asio::io_service& io_service)
: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER))
{
	m_bIsAccepting = false;
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
	static ClientInfoManager* pManager = ClientInfoManager::getInstance();
	std::cout << "클라이언트 접속 종료. 세션 ID: " << nClientInfoID << std::endl;

	pManager->getClient(nClientInfoID)->Socket().close();
	pManager->returnClient(nClientInfoID);

	if (m_bIsAccepting == false)
	{
		PostAccept();
	}
}

void GameNetwork::ProcessPacket(const int nClientInfoID, const char*pData)
{
	static ClientInfoManager* pManager = ClientInfoManager::getInstance();
	PacketHeader* pheader = (PacketHeader*)pData;

	switch (pheader->protocol)
	{
	case PacketType::LOGIN_PACKET:
	{
		PacketLogin* pPacket = (PacketLogin*)pData;
		ClientInfo* pClient = pManager->getClient(nClientInfoID);
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
	}
		break;
	case PacketType::MOVE_PACKET:
	{
		/*PKT_REQ_CHAT* pPacket = (PKT_REQ_CHAT*)pData;

		PKT_NOTICE_CHAT SendPkt;
		SendPkt.Init();
		strncpy_s(SendPkt.szName, MAX_NAME_LEN, m_pClientManager->getClient(nClientInfoID)->GetName(), MAX_NAME_LEN - 1);
		strncpy_s(SendPkt.szMessage, MAX_MESSAGE_LEN, pPacket->szMessage, MAX_MESSAGE_LEN - 1);

		size_t nTotalClientInfoCount = m_vClientInfoList.size();

		for (size_t i = 0; i < nTotalClientInfoCount; ++i)
		{
			if (m_vClientInfoList[i]->Socket().is_open())
			{
				m_vClientInfoList[i]->PostSend(false, SendPkt.nSize, (char*)&SendPkt);
			}
		}*/
	}
		break;
	}

	return;
}

bool GameNetwork::PostAccept()
{
	static ClientInfoManager* pManager = ClientInfoManager::getInstance();
	ClientInfo* pClient = pManager->connectClient();

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