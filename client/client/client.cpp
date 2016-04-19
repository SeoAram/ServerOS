// client.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


//boost�� �޵��� ����
//������ Ŭ���̾�Ʈ �����Ͽ� ó���Ұ�

void setTimer(){
	auto time = boost::chrono::system_clock::now();
	while (1){
		Sleep(1);
		if (time + boost::chrono::seconds(1) < boost::chrono::system_clock::now()){
			time = boost::chrono::system_clock::now();
			std::cout << "===========setTimer==============" << std::endl;
			ClientInfoManager* pClientManager = ClientInfoManager::getInstance();
			for (int i = 0; i < MAX_CONNECT; ++i){

				ClientInfo* pClient = pClientManager->getClient(i);

				pClient->getObject()->move();

				PacketMove pData;
				pData.Init();
				pData.dir_x = pClient->getObject()->m_pvDir->x;
				pData.dir_y = pClient->getObject()->m_pvDir->y;
				pData.dir_z = pClient->getObject()->m_pvDir->z;
				pData.pos_x = pClient->getObject()->m_pvPos->x;
				pData.pos_y = pClient->getObject()->m_pvPos->y;
				pData.pos_z = pClient->getObject()->m_pvPos->z;

				pData.wAxis = pClient->getObject()->getAxis();
				pClient->PostSend(false, pData.packetSize, (char*)&pData);
			}
		}

	}
}

int main()
{

	ClientInfoManager* pClientManager = ClientInfoManager::getInstance();

	char cIPAddr[CHAR_MAX];
	u_short usPort;


	cout << "IPAddress (xxx.xxx.xxx.xxx) : "; cin >> cIPAddr;
	cout << "Port Number : "; cin >> usPort;
	getchar();


	boost::asio::io_service io_service;
	boost::asio::steady_timer timer(io_service);
	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(cIPAddr), usPort);
	
	pClientManager->connect(io_service, endpoint);
	boost::thread thread(boost::bind(&boost::asio::io_service::run, &io_service));

	//�̺�Ʈ ������� ���� �ƴϸ� �׳� �������� �ұ�
	
	setTimer();

	thread.join();
	
	return 0;
}
