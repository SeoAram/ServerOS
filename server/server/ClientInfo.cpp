#include "stdafx.h"
#include "ClientInfo.h"

ClientInfo::ClientInfo(boost::asio::io_service& io_service)
: m_Socket(io_service), m_strand(io_service)
{
	m_pObject = new GameObject(0);
}

ClientInfo::ClientInfo(boost::asio::io_service& io_service, GameNetwork* pGameNet)
: m_Socket(io_service), m_pGameNet(pGameNet), m_strand(io_service)
{
	m_pObject = new GameObject(0);
}

ClientInfo::ClientInfo(unsigned int i, boost::asio::io_service& io_service, GameNetwork* pGameNet)
: m_Socket(io_service), m_pGameNet(pGameNet), m_strand(io_service)
{
	m_pObject = new GameObject(i);
}

ClientInfo::~ClientInfo(){
	while (m_SendDataQueue.empty() == false)
	{
		MemoryPool::getInstance()->pushMemory(m_SendDataQueue.front());
		//delete[] m_SendDataQueue.front();
		m_SendDataQueue.pop();
	}
}

void ClientInfo::setSocketOpt(boost::asio::ip::tcp::no_delay& option){
	m_Socket.set_option(option);
}

void ClientInfo::PostReceive()
{
	m_Socket.async_read_some(
		boost::asio::buffer(m_ReceiveBuffer),
		m_strand.wrap(
			boost::bind(&ClientInfo::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
		)
	);
}

void ClientInfo::setSendQueue(const bool bImmediately, const int nSize, char* pData){
	Data* pSendData = nullptr;

	if (bImmediately == false)
	{
		while ((pSendData = MemoryPool::getInstance()->popMemory()) == nullptr) boost::this_thread::sleep(boost::posix_time::seconds(1));
		memcpy((char*)pSendData->buf, pData, nSize);

	}
	else
	{
		pSendData = (Data*)pData;
		
	}
	m_SendDataQueue.push(pSendData);
	//���� �����۾� �ϰ� �սô� :3c

}

void ClientInfo::PostSend(const bool bImmediately, const int nSize, char* pData){
	Data* pSendData = nullptr;

	m_mutex.lock();
	if (bImmediately == false)
	{
		while ((pSendData = MemoryPool::getInstance()->popMemory()) == nullptr) boost::this_thread::sleep(boost::posix_time::seconds(1));
		memcpy((char*)pSendData->buf, pData, nSize);
		//std::cout << "ClientInfo PostSend(false) - " << m_pObject->getObjId() << " :: " << ((PacketHeader*)pSendData->buf)->id << " :: " << ((PacketHeader*)pSendData->buf)->packetSize << std::endl;
		
		m_SendDataQueue.push(pSendData);
	}
	else
	{
		pSendData = (Data*)pData;
	}

	if (bImmediately == false && m_SendDataQueue.size() > 1)
	{
		m_mutex.unlock();
		return;
	}
	m_mutex.unlock();

	boost::asio::async_write(
		m_Socket, 
		boost::asio::buffer(pSendData->buf, ((PacketHeader*)pSendData->buf)->packetSize),
		m_strand.wrap(
			boost::bind(&ClientInfo::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
			)
		);
}

void ClientInfo::Init()
{
	m_nPacketBufferMark = 0;
	m_pObject->resetObject();
}

void ClientInfo::handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
{
	m_mutex.lock();
	Data* memory = m_SendDataQueue.front();
	m_SendDataQueue.pop();
	m_mutex.unlock();
	MemoryPool::getInstance()->pushMemory(memory);
	
	//std::cout << "send Success~" << m_pObject->getObjId() << std::endl;

	if (m_pObject->m_wState == IniData::getInstance()->getData("GAME_OBJECT_LOGOUT")){
		while (m_SendDataQueue.size() != 0){
			std::cout << m_pObject->getObjId() << " Object socket close, send queue clear" << std::endl;
			memory = m_SendDataQueue.front();
			m_SendDataQueue.pop();
			MemoryPool::getInstance()->pushMemory(memory);
		}
	}

	m_mutex.lock();
	bool tmp = true;
	if (m_SendDataQueue.size() || m_SendDataQueue.empty() == false)
	{
		memory = m_SendDataQueue.front();
		PacketHeader* pHeader = (PacketHeader*)memory->buf;
		//std::cout << "handle_write() - " << m_pObject->getObjId() << " :: " << pHeader->id << " :: " << pHeader->packetSize << std::endl;
		tmp = false;
		m_mutex.unlock();
		PostSend(true, pHeader->packetSize, (char*)memory);
	}
	if (tmp)
		m_mutex.unlock();
}

void ClientInfo::handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
{
	if (error)
	{
		if (error == boost::asio::error::eof || 0 == bytes_transferred)
		{
			std::cout << "Ŭ���̾�Ʈ�� ������ ���������ϴ�" << std::endl;
		}
		else
		{
			std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
		}

		m_pGameNet->CloseClientInfo(m_pObject->getObjId());
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
				m_pGameNet->ProcessPacket(m_pObject->getObjId(), &m_PacketBuffer[nReadData]);

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