// MMO_server.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"

int main()
{
	GameServer gServer;
	boost::asio::io_service io_service;

	gServer.initServer(io_service);
	gServer.startServer(io_service);
	gServer.loopServer(&io_service);
	return 0;
}
