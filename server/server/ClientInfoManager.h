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

	//클라이언트 연결하는 부분인데 이건 그냥 여유있는 클라이언트 찾아서 반환해주는 것으로 변경해야 함.
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

