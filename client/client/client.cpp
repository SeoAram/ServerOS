// client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


//boost로 받도록 수정
//여러개 클라이언트 접속하여 처리할것

void setTimer(){
	auto time = boost::chrono::system_clock::now();
	static short count = 0;
	int moveSet[MAX_CONNECT] = { 0 };
	int moveCheck[MAX_CONNECT] = { 0 };
	while (1){
		Sleep(1);
		if (time + boost::chrono::seconds(1/60) < boost::chrono::system_clock::now()){
			time = boost::chrono::system_clock::now();
			++count;
			//std::cout << "===========setTimer==============" << std::endl;
			ClientInfoManager* pClientManager = ClientInfoManager::getInstance();
			for (int i = 0; i < MAX_CONNECT; ++i){

				ClientInfo* pClient = pClientManager->getClient(i);


				pClient->getObject()->move();
			}

			if (60 <= count){
				count = 0;
				for (int i = 0; i < MAX_CONNECT; ++i){
					++moveCheck[i];

					if (moveSet[i] < moveCheck[i]){

						ClientInfo* pClient = pClientManager->getClient(i);

						std::cout << "id : " << pClient->getObject()->getObjId() << " ";
						pClient->getObject()->m_pvPos->operator<<(std::cout) << " - " << pClient->getObject()->getAxis() << "\n";
						PacketMove pData;
						pData.Init();
						pData.id = pClient->getObject()->getObjId();
						pData.dir_x = pClient->getObject()->m_pvDir->x;
						pData.dir_y = pClient->getObject()->m_pvDir->y;
						pData.dir_z = pClient->getObject()->m_pvDir->z;
						pData.pos_x = pClient->getObject()->m_pvPos->x;
						pData.pos_y = pClient->getObject()->m_pvPos->y;
						pData.pos_z = pClient->getObject()->m_pvPos->z;

						pData.wAxis = pClient->getObject()->getAxis();
						pClient->PostSend(false, pData.packetSize, (char*)&pData);

						moveCheck[i] = 0;
						moveSet[i] = rand() % 60;
						pClient->getObject()->setAxis(rand() % 360);
					}
				
				}
			}
		}

	}
}

int main()
{
	srand(time(0));

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

	//이벤트 등록제로 갈까 아니면 그냥 랜덤으로 할까
	
	char c = 0;

	while ((c = getchar()) == 0);

	setTimer();

	thread.join();
	
	return 0;
}
