#pragma once
#include "stdafx.h"

class GameNetwork
{
public:
	GameNetwork(boost::asio::io_service& io_service);

	~GameNetwork();
	
	void Start();

	void CloseClientInfo(const int nClientInfoID);

	void ProcessPacket(const int nClientInfoID, const char*pData);

private:
	bool PostAccept();

	void handle_accept(ClientInfo* pClientInfo, const boost::system::error_code& error);

	int m_nSeqNumber;

	bool m_bIsAccepting;

	boost::asio::ip::tcp::acceptor m_acceptor;

	ClientInfoManager* m_pClientManager;
};

