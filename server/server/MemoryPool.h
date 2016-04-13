#pragma once
#include "stdafx.h"

#define	PACKET_BUF_SIZE  15 // ¿”Ω√∞¥√º
class SendOver{
public:
	WSAOVERLAPPED	m_Overlapped;
	WSABUF			m_WsaBuf;
	char			m_arPacketBuf[PACKET_BUF_SIZE];
	UINT			m_nOperation;

	SendOver(){
		m_WsaBuf.buf = m_arPacketBuf;
		m_WsaBuf.len = PACKET_BUF_SIZE;
		m_nOperation = NULL;
		ZeroMemory(&m_Overlapped, sizeof(WSAOVERLAPPED));
	}
};

class MemoryPool
{
private:
	MemoryPool();

	std::queue<SendOver*> m_pMemory;
	boost::mutex m_mutexPush;
	boost::mutex m_mutexPop;

public:
	~MemoryPool();
	static MemoryPool* getInstance(){
		static MemoryPool instance;
		return &instance;
	}

	void createMemoryPool();
	char* popMemory(unsigned int size);
};

