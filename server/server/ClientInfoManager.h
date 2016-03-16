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

	//Ŭ���̾�Ʈ �����ϴ� �κ��ε� �̰� �׳� �����ִ� Ŭ���̾�Ʈ ã�Ƽ� ��ȯ���ִ� ������ �����ؾ� ��.
	ClientInfo* connectClient();
};

