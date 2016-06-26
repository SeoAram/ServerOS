#pragma once

#include "stdafx.h"

class ServiceManager
{
public:
	static ServiceManager* getInstance(){
		static ServiceManager instance;
		return &instance;
	}

	unsigned int getMaxService(){ return m_uMaxService; }

	boost::asio::io_service* getIo_Service(unsigned int number){
		if (m_uMaxService <= number){
			std::cout << "io_service object number miss(max : " << m_uMaxService << ", want service num : " << number << "\n";
			std::cout << "return io_service number : " << m_uMaxService-1 << "\n";
			return (m_v_io_service[m_uMaxService - 1]);
		}
		return m_v_io_service[number];
	}

	~ServiceManager();
private:
	std::vector<boost::asio::io_service*> m_v_io_service;
	const unsigned int m_uMaxService;
	ServiceManager();
};

