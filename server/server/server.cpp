// MMO_server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

int main()
{
	ClientInfoManager* cpManager = ClientInfoManager::getInstance();
	boost::asio::io_service io_service;

	cpManager->initializeClientInfoManager(io_service);

	NetworkEngine server(io_service);

	server.acceptThread();
	io_service.run();

	std::cout << "네트웍 접속 종료" << std::endl;

	getchar();
	return 0;
}
