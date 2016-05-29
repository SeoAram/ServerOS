#pragma once
#include "stdafx.h"

class GameNetwork
{
	
public:
	void Start();

	void CloseClientInfo(const unsigned int nClientInfoID);

	void ProcessPacket(const unsigned int nClientInfoID, const char*pData);

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

	boost::mutex m_mutex;
	boost::mutex* m_pMutex;
	boost::mutex::scoped_lock* m_pLock;
	boost::asio::ip::tcp::acceptor m_acceptor;
	ClientInfoManager* m_pClientManager;

	boost::thread_group* m_pTheadPool;

};

