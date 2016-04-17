// client.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
//

#include "stdafx.h"


//boost�� �޵��� ����
//������ Ŭ���̾�Ʈ �����Ͽ� ó���Ұ�

void onTimer(const boost::system::error_code& error, boost::asio::steady_timer* pTimer);

void setTimer(boost::asio::steady_timer* pTimer){
	boost::chrono::milliseconds ms(1000);
	pTimer->expires_from_now(boost::asio::steady_timer::duration());
	pTimer->async_wait(boost::bind(onTimer, boost::asio::placeholders::error, pTimer));

	Sleep(1000);
	std::cout << "===========setTimer==============" << std::endl;
}

void onTimer(const boost::system::error_code& error, boost::asio::steady_timer* pTimer){
	
	if (!error){
		ClientInfoManager* pClientManager = ClientInfoManager::getInstance();
		for (int i = 0; i < MAX_CONNECT; ++i){
			ClientInfo* pClient = pClientManager->getClient(i);
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
	else{
		std::cout << "Error No : " << error.value() << " Error Message : " << error.message() << std::endl;
	}

	setTimer(pTimer);


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
	
	setTimer(&timer);

	thread.join();
	
	return 0;
}
