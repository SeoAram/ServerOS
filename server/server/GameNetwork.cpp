#include "stdafx.h"
#include "GameNetwork.h"


GameNetwork::GameNetwork(boost::asio::io_service& io_service)
: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
m_pClientManager(ClientInfoManager::getInstance())
{
	m_bIsAccepting = false;
}

GameNetwork::~GameNetwork()
{

}

void GameNetwork::Start()
{
	std::cout << "���� ����....." << std::endl;

	PostAccept();
}

void GameNetwork::CloseClientInfo(const int nClientInfoID)
{
	std::cout << "Ŭ���̾�Ʈ ���� ����. ���� ID: " << nClientInfoID << std::endl;

	m_pClientManager->getClient(nClientInfoID)->Socket().close();
	m_pClientManager->returnClient(nClientInfoID);

	if (m_bIsAccepting == false)
	{
		PostAccept();
	}
}

void GameNetwork::ProcessPacket(const int nClientInfoID, const char*pData)
{
	PacketHeader* pheader = (PacketHeader*)pData;

	switch (pheader->protocol)
	{
	case PacketType::LOGIN_PACKET:
	{
		PacketLogin* pPacket = (PacketLogin*)pData;

		std::cout << "Ŭ���̾�Ʈ �α��� ���� Id: " << m_pClientManager->getClient(nClientInfoID)->getObject()->getObjId() << std::endl;
/*
		PKT_RES_IN SendPkt;
		SendPkt.Init();
		SendPkt.bIsSuccess = true;

		//���� ���� �� ��Ŷ ����
		m_pClientManager->getClient(nClientInfoID)->PostSend(false, SendPkt.nSize, (char*)&SendPkt);*/
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
	ClientInfo* pClient = m_pClientManager->connectClient();

	m_bIsAccepting = true;

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
		std::cout << "Ŭ���̾�Ʈ ���� ����. ClientInfoID: " << pClientInfo->getObject()->getObjId() << std::endl;

		pClientInfo->Init();
		pClientInfo->PostReceive();

		PostAccept();
	}
	else
	{
		std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
	}
}