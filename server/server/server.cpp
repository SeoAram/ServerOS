// MMO_server.cpp : �ܼ� ���� ���α׷��� ���� �������� �����մϴ�.
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
		std::cout << "Ŭ���̾�Ʈ ���� ���....." << std::endl;

		//Ŭ���̾�Ʈ �����ؼ� ��ȯ���ִ� �κ�
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
			std::cout << "Ŭ���̾�Ʈ ���� ����" << std::endl;

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

	std::cout << "��Ʈ�� ���� ����" << std::endl;

	getchar();
	return 0;
}
