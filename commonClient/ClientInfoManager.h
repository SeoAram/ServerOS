#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <boost/asio.hpp>

#include "../common/iniRead.h"
#include "../common/protocol.h"
#include "../common/PointVector3D.h"

#include "ClientInfo.h"


#define MAX_CONNECT 100

class ClientInfoManager
{
private:
	vector<ClientInfo*> m_vClient;

	ClientInfoManager()
	{
	}
public:
	~ClientInfoManager()
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

	static ClientInfoManager* getInstance(){
		static ClientInfoManager instance;
		return &instance;
	}

	bool pushClientInfo(boost::asio::io_service& io_service){
		
	}

	ClientInfo* getClient(const unsigned int nObjId){
		return m_vClient[nObjId];
	}

	void connect(boost::asio::io_service& io_service, boost::asio::ip::tcp::endpoint& endpoint){
		for (int i = 0; i < MAX_CONNECT; ++i)
			m_vClient.push_back(new ClientInfo(i, io_service, endpoint));
	}
};

