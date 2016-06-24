// MMO_server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int main()
{
	GameServer gServer;

	std::vector<boost::asio::io_service*> vector_io_service;
	boost::asio::io_service io_service;

	const unsigned int threadCount = boost::thread::hardware_concurrency() - 1;

	for (int i = 0; i < threadCount; ++i){
		vector_io_service.push_back(new boost::asio::io_service());
	}
	

	gServer.initServer(io_service);
	gServer.startServer(io_service);
	gServer.loopServer(&io_service);

	
	return 0;
}
