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

	void connectThread(const unsigned int threadId);

	bool PostAccept();

	bool PostAccept(const unsigned int threadId);

	void handle_accept(ClientInfo* pClientInfo, const boost::system::error_code& error);

	void handle_accept(ClientInfo* pClientInfo, const unsigned int threadId, const boost::system::error_code& error);

	bool m_bIsAccepting;

	boost::mutex m_mutex;
	boost::mutex* m_pMutex;
	boost::mutex::scoped_lock* m_pLock;
	boost::asio::ip::tcp::acceptor m_acceptor;
	boost::asio::io_service& m_io_service;
	boost::asio::io_service::strand m_strand;
	ClientInfoManager* m_pClientManager;
	GameEventProcess* m_pEventProcess;

	vector<boost::thread*> m_vThread;
	boost::thread_group m_threadGroup;

	const unsigned int m_uThreadCount;
};

