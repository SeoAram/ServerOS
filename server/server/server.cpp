// MMO_server.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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

	std::cout << "��Ʈ�� ���� ����" << std::endl;

	getchar();
	return 0;
}
