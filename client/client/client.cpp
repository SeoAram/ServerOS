// client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


//boost로 받도록 수정
//여러개 클라이언트 접속하여 처리할것

int main()
{

	std::vector<ClientInfo*> vecClient;

	char cIPAddr[CHAR_MAX];
	u_short usPort;


	cout << "IPAddress (xxx.xxx.xxx.xxx) : "; cin >> cIPAddr;
	cout << "Port Number : "; cin >> usPort;
	getchar();


	boost::asio::io_service io_service;
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(cIPAddr), usPort);
	
	for (int i = 0; i < CONNECT_SOCKET; ++i){
		vecClient.push_back(new ClientInfo(i, io_service, endpoint));
		std::cout << i << ":: connect " << std::endl;
	}
	boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_service));

	//io_service.run();
	//while (true);
	thread.join();
	
	return 0;
}
