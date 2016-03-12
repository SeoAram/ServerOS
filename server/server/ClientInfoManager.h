#pragma once
#include "stdafx.h"

class ClientInfoManager
{
private:
	queue<int> m_qLeaveId;
	vector<ClientInfo*> m_vClient;
	ClientInfoManager();

	unsigned int m_uConnCount;
public:
	~ClientInfoManager();

	static ClientInfoManager* getInstance(){
		static ClientInfoManager instance;
		return &instance;
	}

	unsigned int getConnectCount(){ return m_uConnCount; }
	bool addConnextCount(){ if (m_uConnCount < MAX_CONNECT_CLIENT) { m_uConnCount++; return true; } return false; }

	void connectClient(boost::asio::io_service& io_service);
};

