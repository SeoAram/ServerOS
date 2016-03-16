// MMO_server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
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
