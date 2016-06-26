#include "stdafx.h"
#include "ClientInfoManager.h"


ClientInfoManager::ClientInfoManager()
:m_bInit(false)
{
	m_pMutexPop = new boost::mutex();
	m_pMutexPush = new boost::mutex();
}


ClientInfoManager::~ClientInfoManager()
{
	for (size_t i = 0; i < m_vClient.size(); ++i)
	{
		if (m_vClient[i]->Socket().is_open())
		{
			m_vClient[i]->Socket().close();
		}

		delete m_vClient[i];
	}
}


//accept에서 connect처리 해야 함
ClientInfo* ClientInfoManager::connectClient(){
	lockPop();
	if (m_qWaitNum.size() == 0){
		unlockPop();
		return nullptr;
	}
	
	unsigned int nConnId = m_qWaitNum.front();

	m_qWaitNum.pop();
	unlockPop();
	return m_vClient[nConnId];
}

ClientInfo* ClientInfoManager::connectClient(const unsigned int nObjId, const unsigned int nMax){
	lockPop();
	if (m_qWaitNum.size() == 0){
		unlockPop();
		return nullptr;
	}
	unsigned int nConnId = m_qWaitNum.front();
	if (nConnId % nMax != nConnId){
		unlockPop();
		return nullptr;
	}

	m_qWaitNum.pop();
	unlockPop();
	return m_vClient[nConnId];
}

bool ClientInfoManager::initClientInfoManager(){
	ServiceManager* pManager = ServiceManager::getInstance();
	if (!m_bInit){
		const unsigned int maxService = pManager->getMaxService();
		for (unsigned int i = 0; i < MAX_CONNECT_CLIENT; ++i){
			m_vClient.push_back(new ClientInfo(	i, *(pManager->getIo_Service(i%maxService)), GameNetwork::getInstance()));
			m_qWaitNum.push(i);
		}
		cout << "Create Object :: " << m_vClient.size() << " :: " << m_qWaitNum.front() << endl;
		m_bInit = true;
		return true;
	}
	return false;
}

bool ClientInfoManager::initClientInfoManager(boost::asio::io_service& io_service){
	if (!m_bInit){
		for (unsigned int i = 0; i < MAX_CONNECT_CLIENT; ++i){
			m_vClient.push_back(new ClientInfo(i, io_service, GameNetwork::getInstance(io_service)));
			m_qWaitNum.push(i);
		}
		cout << "Create Object :: " << m_vClient.size() << " :: " << m_qWaitNum.front() << endl;
		m_bInit = true;
		return true;
	}
	return false;
}


bool ClientInfoManager::initClientInfoManager(boost::asio::ip::tcp::acceptor& acceptor){
	if (!m_bInit){
		for (int i = 0; i < MAX_CONNECT_CLIENT; ++i)
		{
			ClientInfo* pClientInfo = new ClientInfo(i, acceptor.get_io_service(), GameNetwork::getInstance(acceptor.get_io_service()));
			m_vClient.push_back(pClientInfo);
			m_qWaitNum.push(i);
		}
		cout << "Create Object :: " << m_vClient.size() << " :: " << m_qWaitNum.front() << endl;
		m_bInit = true;
		return true;
	}
	return false;
}
