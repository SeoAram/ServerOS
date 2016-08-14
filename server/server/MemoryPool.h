#pragma once
#include "stdafx.h"

#define	PACKET_BUF_SIZE  128 // ¿”Ω√∞¥√º

struct Data{
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
	boost::mutex* m_pMutexPush;
	boost::mutex* m_pMutexPop;

	Node* head;
	Node* tail;

	const unsigned int MAX_MEMORY_SIZE = 300000;

	void lockPop(){ m_pMutexPop->lock(); }
	void unlockPop(){ m_pMutexPop->unlock(); }
	void lockPush(){ m_pMutexPush->lock(); }
	void unlockPush(){ m_pMutexPush->unlock(); }

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

