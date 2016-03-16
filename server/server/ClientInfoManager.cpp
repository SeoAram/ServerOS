#include "stdafx.h"
#include "ClientInfoManager.h"


ClientInfoManager::ClientInfoManager()
:m_uConnCount(0), m_bInit(false)
{
}


ClientInfoManager::~ClientInfoManager()
{
}


//accept���� connectó�� �ؾ� ��
ClientInfo* ClientInfoManager::connectClient(boost::asio::io_service& io_service){
	return nullptr;
}

bool ClientInfoManager::initializeClientInfoManager(boost::asio::io_service& io_service){
	if (!m_bInit){
		for (unsigned int i = 0; i < MAX_CONNECT_CLIENT; ++i){
			m_vClient.push_back(new ClientInfo(i, io_service));
		}
		m_bInit = true;
		return true;
	}
	return false;
}