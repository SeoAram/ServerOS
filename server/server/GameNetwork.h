#pragma once
#include "stdafx.h"

class GameNetwork
{
public:
	GameNetwork(boost::asio::io_service& io_service);

	~GameNetwork();

	void Init(const int nMaxClientInfoCount);

	void Start();

	void CloseClientInfo(const int nClientInfoID);

	void ProcessPacket(const int nClientInfoID, const char*pData);

private:
	bool PostAccept();

	void handle_accept(ClientInfo* pClientInfo, const boost::system::error_code& error);

	int m_nSeqNumber;

	bool m_bIsAccepting;

	boost::asio::ip::tcp::acceptor m_acceptor;

	std::vector< ClientInfo* > m_vClientInfoList;
	std::deque< int > m_ClientInfoQueue;
};

