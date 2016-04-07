#pragma once
#include "stdafx.h"

class GameNetwork
{
	
public:
	void Start();

	void CloseClientInfo(const int nClientInfoID);

	void ProcessPacket(const int nClientInfoID, const char*pData);

	static GameNetwork* getInstance(boost::asio::io_service& io_service){
		static GameNetwork instance(io_service);
		return &instance;
	}

private:
	GameNetwork(boost::asio::io_service& io_service);

	~GameNetwork();

	bool PostAccept();

	void handle_accept(ClientInfo* pClientInfo, const boost::system::error_code& error);

	bool m_bIsAccepting;

	boost::asio::ip::tcp::acceptor m_acceptor;

};

