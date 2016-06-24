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

	static GameNetwork* getInstance(std::vector<boost::asio::io_service*>& v_io_service){
		static GameNetwork instance(v_io_service);
		return &instance;
	};

private:
	GameNetwork(boost::asio::io_service& io_service);
	GameNetwork(std::vector<boost::asio::io_service*>& v_io_service);

	~GameNetwork();

	//void connectThread(const unsigned int threadId);
	void connectThread(const unsigned int threadId, boost::asio::io_service& io_service);

	bool PostAccept();

	bool PostAccept(const unsigned int threadId);

	void handle_accept(ClientInfo* pClientInfo, const boost::system::error_code& error);

	void handle_accept(ClientInfo* pClientInfo, const unsigned int threadId, const boost::system::error_code& error);

	bool m_bIsAccepting;

	boost::mutex m_mutex;
	boost::mutex* m_pMutex;
	boost::asio::ip::tcp::acceptor m_acceptor;
	std::vector<boost::asio::ip::tcp::acceptor*> m_vAcceptor;
	ClientInfoManager* m_pClientManager;

	boost::thread_group* m_pTheadPool;

	vector<boost::thread*> m_vThread;

	boost::asio::io_service* m_io_service;

	const unsigned int m_uThreadCount;
};

