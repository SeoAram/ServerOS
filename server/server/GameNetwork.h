#pragma once
#include "stdafx.h"

class GameNetwork
{
public:
	GameNetwork(boost::asio::io_service& io_service)
		: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER))
	{
		m_bIsAccepting = false;
	}

	~GameNetwork()
	{
		for (size_t i = 0; i < m_vClientInfoList.size(); ++i)
		{
			if (m_vClientInfoList[i]->Socket().is_open())
			{
				m_vClientInfoList[i]->Socket().close();
			}

			delete m_vClientInfoList[i];
		}
	}

	void Init(const int nMaxClientInfoCount)
	{
		for (int i = 0; i < nMaxClientInfoCount; ++i)
		{
			ClientInfo* pClientInfo = new ClientInfo(i, m_acceptor.get_io_service());
			m_vClientInfoList.push_back(pClientInfo);
			m_ClientInfoQueue.push_back(i);
		}
	}

	void Start()
	{
		std::cout << "서버 시작....." << std::endl;

		PostAccept();
	}

	void CloseClientInfo(const int nClientInfoID)
	{
		std::cout << "클라이언트 접속 종료. 세션 ID: " << nClientInfoID << std::endl;

		m_vClientInfoList[nClientInfoID]->Socket().close();

		m_ClientInfoQueue.push_back(nClientInfoID);

		if (m_bIsAccepting == false)
		{
			PostAccept();
		}
	}

	void ProcessPacket(const int nClientInfoID, const char*pData)
	{
		/*PACKET_HEADER* pheader = (PACKET_HEADER*)pData;

		switch (pheader->nID)
		{
		case REQ_IN:
		{
					   PKT_REQ_IN* pPacket = (PKT_REQ_IN*)pData;
					   m_vClientInfoList[nClientInfoID]->SetName(pPacket->szName);

					   std::cout << "클라이언트 로그인 성공 Name: " << m_vClientInfoList[nClientInfoID]->GetName() << std::endl;

					   PKT_RES_IN SendPkt;
					   SendPkt.Init();
					   SendPkt.bIsSuccess = true;

					   m_vClientInfoList[nClientInfoID]->PostSend(false, SendPkt.nSize, (char*)&SendPkt);
		}
			break;
		case REQ_CHAT:
		{
						 PKT_REQ_CHAT* pPacket = (PKT_REQ_CHAT*)pData;

						 PKT_NOTICE_CHAT SendPkt;
						 SendPkt.Init();
						 strncpy_s(SendPkt.szName, MAX_NAME_LEN, m_vClientInfoList[nClientInfoID]->GetName(), MAX_NAME_LEN - 1);
						 strncpy_s(SendPkt.szMessage, MAX_MESSAGE_LEN, pPacket->szMessage, MAX_MESSAGE_LEN - 1);

						 size_t nTotalClientInfoCount = m_vClientInfoList.size();

						 for (size_t i = 0; i < nTotalClientInfoCount; ++i)
						 {
							 if (m_vClientInfoList[i]->Socket().is_open())
							 {
								 m_vClientInfoList[i]->PostSend(false, SendPkt.nSize, (char*)&SendPkt);
							 }
						 }
		}
			break;
		}
*/
		return;
	}


private:
	bool PostAccept()
	{
		if (m_ClientInfoQueue.empty())
		{
			m_bIsAccepting = false;
			return false;
		}

		m_bIsAccepting = true;
		int nClientInfoID = m_ClientInfoQueue.front();

		m_ClientInfoQueue.pop_front();

		m_acceptor.async_accept(m_vClientInfoList[nClientInfoID]->Socket(),
			boost::bind(&GameNetwork::handle_accept,
			this,
			m_vClientInfoList[nClientInfoID],
			boost::asio::placeholders::error)
			);

		return true;
	}

	void handle_accept(ClientInfo* pClientInfo, const boost::system::error_code& error)
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





	int m_nSeqNumber;

	bool m_bIsAccepting;

	boost::asio::ip::tcp::acceptor m_acceptor;

	std::vector< ClientInfo* > m_vClientInfoList;
	std::deque< int > m_ClientInfoQueue;
};

