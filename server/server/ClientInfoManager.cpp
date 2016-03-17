#include "stdafx.h"
#include "ClientInfoManager.h"


ClientInfoManager::ClientInfoManager()
:m_bInit(false)
{
}


ClientInfoManager::~ClientInfoManager()
{
}


//accept에서 connect처리 해야 함
ClientInfo* ClientInfoManager::connectClient(){
	if (m_qWaitNum.size() == 0)
		return nullptr;
	
	unsigned int nConnId = m_qWaitNum.front();

	m_qWaitNum.pop();
	return m_vClient[nConnId];
}

bool ClientInfoManager::initClientInfoManager(boost::asio::io_service& io_service){
	if (!m_bInit){
		for (unsigned int i = 0; i < MAX_CONNECT_CLIENT; ++i){
			m_vClient.push_back(new ClientInfo(i, io_service));
			m_qWaitNum.push(i);
		}
		m_bInit = true;
		return true;
	}
	return false;
}

void ClientInfoManager::closeClient(const unsigned int nObjId){
	cout << "Client Close :: " << nObjId << endl;
	m_vClient[nObjId]->Socket().close();
	m_qWaitNum.push(nObjId);
}