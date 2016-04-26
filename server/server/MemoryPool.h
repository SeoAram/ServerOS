#pragma once
#include "stdafx.h"

#define	PACKET_BUF_SIZE  64 // ¿”Ω√∞¥√º

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

	const unsigned int MAX_MEMORY_SIZE = 300000;
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

