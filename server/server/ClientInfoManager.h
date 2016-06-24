#pragma once
#include "stdafx.h"

class ClientInfoManager
{
private:
	vector<ClientInfo*> m_vClient;
	queue<unsigned int> m_qWaitNum;
	bool m_bInit;

	boost::mutex* m_pMutexPush;
	boost::mutex* m_pMutexPop;

	ClientInfoManager();

	void lockPop(){ m_pMutexPop->lock(); }
	void unlockPop(){ m_pMutexPop->unlock(); }
	void lockPush(){ m_pMutexPush->lock(); }
	void unlockPush(){ m_pMutexPush->unlock(); }
public:
	~ClientInfoManager();

	static ClientInfoManager* getInstance(){
		static ClientInfoManager instance;
		return &instance;
	}

	bool initClientInfoManager(boost::asio::io_service& io_service);
	bool initClientInfoManager(std::vector<boost::asio::io_service*>& v_io_service);
	bool initClientInfoManager(boost::asio::ip::tcp::acceptor& acceptor);

	//Ŭ���̾�Ʈ �����ϴ� �κ��ε� �̰� �׳� �����ִ� Ŭ���̾�Ʈ ã�Ƽ� ��ȯ���ִ� ������ �����ؾ� ��.
	ClientInfo* connectClient();
	ClientInfo* connectClient(const unsigned int nObjId, const unsigned int nMax);

	ClientInfo* getClient(const unsigned int nObjId){
		return m_vClient[nObjId];
	}
	void returnClient(const unsigned int nObjId){ 
		lockPush();
		m_qWaitNum.push(nObjId); 
		unlockPush();
	}
};

