#pragma once
#include "stdafx.h"

#define	PACKET_BUF_SIZE  128 // ¿”Ω√∞¥√º
class SendOver{
public:
	/*WSAOVERLAPPED	m_Overlapped;
	WSABUF			m_WsaBuf;
	char			m_arPacketBuf[PACKET_BUF_SIZE];
	UINT			m_nOperation;

	SendOver(){
		m_WsaBuf.buf = m_arPacketBuf;
		m_WsaBuf.len = PACKET_BUF_SIZE;
		m_nOperation = NULL;
		ZeroMemory(&m_Overlapped, sizeof(WSAOVERLAPPED));
	}*/
};

struct Data{
	unsigned int objId;
	MsgType msgType;
	char* buf;
	Data(){ buf = new char[PACKET_BUF_SIZE]; }
};

struct Node{
	Data data;
	Node* next;
};

class MemoryPool
{
private:
	MemoryPool();

	std::queue<Data*> m_pMemory;
	boost::mutex m_mutexPush;
	boost::mutex m_mutexPop;

	Node* head;
	Node* tail;

	const unsigned int MAX_MEMORY_SIZE = 30000;
public:
	~MemoryPool();
	static MemoryPool* getInstance(){
		static MemoryPool instance;
		return &instance;
	}

	void createMemoryPool();
	Data* popMemory();
	void pushMemory(Data* memory);

	Node* getMemory();
	void returnMemory(Node* memory);
};

