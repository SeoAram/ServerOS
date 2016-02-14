#pragma once

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

struct NODE{
	SendOver m_stBuf;
	NODE* m_pNext;
};

class MemoryPool
{
private:
	MemoryPool();
	~MemoryPool();

	NODE* head;
	NODE* tail;

	CRITICAL_SECTION m_csPopLock;
	CRITICAL_SECTION m_csPushLock;
public:
	static MemoryPool* getInstance(){
		static MemoryPool instance;
		return &instance;
	}

	void createMemoryPool();
	NODE* getMemory();
	void returnMemory(NODE* memory);
};

