#include "stdafx.h"
#include "ServiceManager.h"


ServiceManager::ServiceManager():
m_uMaxService(boost::thread::hardware_concurrency()-1)
{
	for (int i = 0; i < m_uMaxService; ++i){
		m_v_io_service.push_back(new boost::asio::io_service());
	}

	std::cout << "io_service create :: " << m_v_io_service.size() << " / " << m_uMaxService << " \n";
}


ServiceManager::~ServiceManager()
{
}
