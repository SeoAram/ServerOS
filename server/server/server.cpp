// MMO_server.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"


class TCP_Server
{
public:
	TCP_Server(boost::asio::io_service& io_service)
		: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER))
	{
		m_pSession = nullptr;
		ClientInfoManager* cpManager = ClientInfoManager::getInstance();
		cpManager->initializeClientInfoManager(io_service);
		StartAccept();
	}

	~TCP_Server()
	{
		if (m_pSession != nullptr)
		{
			delete m_pSession;
		}
	}

private:
	void StartAccept()
	{
		ClientInfoManager* cpManager = ClientInfoManager::getInstance();
		std::cout << "클라이언트 접속 대기....." << std::endl;

		//클라이언트 접속해서 반환해주는 부분
		m_pSession = cpManager->connectClient();
		//m_pSession = new ClientInfo(0, m_acceptor.get_io_service());

		m_acceptor.async_accept(m_pSession->Socket(),
			boost::bind(&TCP_Server::handle_accept,
			this,
			m_pSession,
			boost::asio::placeholders::error)
			);
	}

	void handle_accept(ClientInfo* pSession, const boost::system::error_code& error)
	{
		if (!error)
		{
			std::cout << "클라이언트 접속 성공" << std::endl;

			pSession->PostReceive();
		}
	}

	int m_nSeqNumber;
	boost::asio::ip::tcp::acceptor m_acceptor;
	ClientInfo* m_pSession;
};

int main()
{
	ClientInfoManager * cpManager = ClientInfoManager::getInstance();
	boost::asio::io_service io_service;

	TCP_Server server(io_service);

	io_service.run();

	std::cout << "네트웍 접속 종료" << std::endl;

	getchar();
	return 0;
}
