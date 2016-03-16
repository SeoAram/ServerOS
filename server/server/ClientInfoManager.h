#pragma once
#include "stdafx.h"

class ClientInfoManager
{
private:
	queue<int> m_qLeaveId;
	unsigned int m_uConnCount;
	vector<ClientInfo*> m_vClient;
	queue<int> m_qConnectNum;
	bool m_bInit;

	ClientInfoManager();
public:
	~ClientInfoManager();

	static ClientInfoManager* getInstance(){
		static ClientInfoManager instance;
		return &instance;
	}

	bool initializeClientInfoManager(boost::asio::io_service& io_service);

	ClientInfo* connectClient(boost::asio::io_service& io_service);
};

