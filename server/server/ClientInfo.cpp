#include "stdafx.h"
#include "ClientInfo.h"

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
			std::cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�" << std::endl;
		}
		else
		{
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}
		//�̷� ������ ���� �����Դϴ�. ���� ���ô�.
		GameMap::getInstance()->deleteObjId(m_pObject->m_wBlockX, m_pObject->m_wBlockZ, m_pObject->getObjId());
		ClientInfoManager::getInstance()->closeClient(m_pObject->getObjId());
	}
	else
	{
		memcpy(&m_PacketBuffer[m_nPacketBufferMark], m_ReceiveBuffer.data(), bytes_transferred);

		int nPacketData = m_nPacketBufferMark + bytes_transferred;
		int nReadData = 0;

		while (nPacketData > 0)
		{
			if (nPacketData < sizeof(PacketHeader))
			{
				break;
			}

			PacketHeader* pHeader = (PacketHeader*)&m_PacketBuffer[nReadData];

			if (pHeader->packetSize <= nPacketData)
			{
				//packet process�� �����ߵ�
				PacketProcess* pProcess = PacketProcess::getInstance();
				//m_pServer->ProcessPacket(m_nSessionID, &m_PacketBuffer[nReadData]);

				nPacketData -= pHeader->packetSize;
				nReadData += pHeader->packetSize;
			}
			else
			{
				break;
			}
		}

		if (nPacketData > 0)
		{
			char TempBuffer[MAX_RECEIVE_BUFFER_LEN] = { 0, };
			memcpy(&TempBuffer[0], &m_PacketBuffer[nReadData], nPacketData);
			memcpy(&m_PacketBuffer[0], &TempBuffer[0], nPacketData);
		}

		m_nPacketBufferMark = nPacketData;

		//���� �ֺ��� �ִ� Ŭ�󿡰� �����͸� �������ִ� ������ �����ô�.
		PostReceive();
	}
}
