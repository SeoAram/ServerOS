#pragma once
#include "stdafx.h"

class TCP_Server;

class ClientInfoManager
{
private:
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
	bool initClientInfoManager(boost::asio::ip::tcp::acceptor& acceptor);

	//Ŭ���̾�Ʈ �����ϴ� �κ��ε� �̰� �׳� �����ִ� Ŭ���̾�Ʈ ã�Ƽ� ��ȯ���ִ� ������ �����ؾ� ��.
	ClientInfo* connectClient();

	ClientInfo* getClient(const unsigned int nObjId){
		return m_vClient[nObjId];
	}
	void returnClient(const unsigned int nObjId){ m_qWaitNum.push(nObjId); }
};

