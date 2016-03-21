#include "stdafx.h"
#include "ClientInfo.h"


//ClientInfo::ClientInfo()
//{
//}

ClientInfo::ClientInfo(boost::asio::io_service& io_service)
: m_Socket(io_service)
{
	m_pObject = new GameObject(0);
}

ClientInfo::ClientInfo(unsigned int i, boost::asio::io_service& io_service)
: m_Socket(io_service)
{
	m_pObject = new GameObject(i);
}

void ClientInfo::PostReceive()
{
	memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));

	m_Socket.async_read_some
		(
		boost::asio::buffer(m_ReceiveBuffer),
		boost::bind(&ClientInfo::handle_receive, this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred)

		);

}

void ClientInfo::handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
{
}

void ClientInfo::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof)
		{
			std::cout << "클라이언트와 연결이 끊어졌습니다" << std::endl;
		}
		else
		{
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
		//이런 구조는 에비 지지입니다. 쓰지 맙시다.
		GameMap::getInstance()->deleteObjId(m_pObject->m_pPosition->x, m_pObject->m_pPosition->z, m_pObject->getObjId());
		ClientInfoManager::getInstance()->closeClient(m_pObject->getObjId());
	}
	else
	{
		const std::string strRecvMessage = m_ReceiveBuffer.data();
		std::cout << "클라이언트에서 받은 메시지: " << strRecvMessage << ", 받은 크기: " << bytes_transferred << std::endl;

		char szMessage[128] = { 0, };
		sprintf_s(szMessage, 128 - 1, "Re:%s", strRecvMessage.c_str());
		m_WriteMessage = szMessage;

		boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage),
			boost::bind(&ClientInfo::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
			);

		//이런 구조는 에비 지지입니다. 쓰지 맙시다222222
		//ㅓ클라리언트에게 모든 것을 보내줍시다
		ClientInfoManager::getInstance();
		PostReceive();
	}
}
