#pragma once
#include <iostream>
#include <queue>
#include <vector>
#include <boost/asio.hpp>

#include "../common/iniRead.h"
#include "../common/protocol.h"
#include "../common/PointVector3D.h"

#include "ClientInfo.h"

class ClientInfoManager
{
private:
	vector<ClientInfo*> m_vClient;
	queue<unsigned int> m_qWaitNum;
	bool m_bInit;

	ClientInfoManager() :m_bInit(false)
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

	//클라이언트 연결하는 부분인데 이건 그냥 여유있는 클라이언트 찾아서 반환해주는 것으로 변경해야 함.
	ClientInfo* connectClient(){
		if (m_qWaitNum.size() == 0)
			return nullptr;

		unsigned int nConnId = m_qWaitNum.front();

		m_qWaitNum.pop();
		return m_vClient[nConnId];
	}

	ClientInfo* getClient(const unsigned int nObjId){
		return m_vClient[nObjId];
	}
	void returnClient(const unsigned int nObjId){ m_qWaitNum.push(nObjId); }
};

