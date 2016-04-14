// client.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


//boost�� �޵��� ����
//������ Ŭ���̾�Ʈ �����Ͽ� ó���Ұ�

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
