#pragma once
#include "stdafx.h"

class TCP_Server;

class ClientInfoManager
{
private:
	queue<int> m_qLeaveId;
	vector<ClientInfo*> m_vClient;
	queue<unsigned int> m_qWaitNum;
	bool m_bInit;

	ClientInfoManager();
public:
	~ClientInfoManager();

	static ClientInfoManager* getInstance(){
		static ClientInfoManager instance;
		return &instance;
	}

	bool initClientInfoManager(boost::asio::io_service& io_service);

	//클라이언트 연결하는 부분인데 이건 그냥 여유있는 클라이언트 찾아서 반환해주는 것으로 변경해야 함.
	ClientInfo* connectClient();
};

