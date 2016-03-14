#include "stdafx.h"
#include "ClientInfoManager.h"


ClientInfoManager::ClientInfoManager()
:m_uConnCount(0)
{
	//m_qLeaveId.emplace(MAX_CONNECT_CLIENT);

	//boost::asio::ip::tcp::acceptor m_acceptor(boost::asio::io_service(), boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 31401));
	for (unsigned int i = 0; i < MAX_CONNECT_CLIENT; ++i){
		m_vClient.push_back((ClientInfo*)malloc( sizeof(ClientInfo)));
		//m_vClient.push_back(new ClientInfo(i, m_acceptor.get_io_service()));
	}
}


ClientInfoManager::~ClientInfoManager()
{
}


//accept���� connectó�� �ؾ� ��
ClientInfo* ClientInfoManager::connectClient(boost::asio::io_service& io_service){
	ClientInfo c(getConnectCount(), io_service);
	int i = getConnectCount();
	memcpy(m_vClient[getConnectCount()], &c, sizeof(ClientInfo));
	addConnextCount();
	return m_vClient[i];
}