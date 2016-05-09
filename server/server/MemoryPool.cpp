#include "stdafx.h"
#include "MemoryPool.h"


MemoryPool::MemoryPool()
{
	head = new Node;
	tail = head;
	m_pMutexPop = new boost::mutex();
	m_pMutexPush = new boost::mutex();
}


MemoryPool::~MemoryPool()
{
}


void MemoryPool::createMemoryPool(){
	Node* node;
	for (unsigned int i = 0; i < MAX_MEMORY_SIZE; ++i){
		m_pMemory.push(new Data);
		/*node = new Node;
		tail->next = node;
		tail = node;*/
	}
	//tail->next = nullptr;
	std::cout << "Create Success MemoryPool" << std::endl;
}

Data* MemoryPool::popMemory(){
	lockPop();
	if (m_pMemory.size() == 0)
	{
		unlockPop();
		return nullptr;
	}
	// 잠금 m_mutexPop;
	Data* memory = m_pMemory.front();
	m_pMemory.pop();
	// 해제 m_mutexPop;
	unlockPop();
	return memory;
}
void MemoryPool::pushMemory(Data* memory){
	//push lock필요
	//잠금 m_mutexPush;
	lockPush();
	m_pMemory.push(memory);
	unlockPush();
	if (m_pMemory.size() % 300 == 0)
		std::cout << "push Memory Size : " << m_pMemory.size() << "\n";
	//해제 m_mutexPush;
}

Node* MemoryPool::getMemory(){
	Node* curNode;

	// m_mutextPop;
	curNode = head;
	if (curNode->next == nullptr){
		
		curNode = nullptr;
	}
	else{
		head = head->next;
		curNode->next = nullptr;

	}
	//해제 m_mutextPop;
	return curNode;
}

void MemoryPool::returnMemory(Node* memory){
	Node* lastNode;

	// m_mutextPush;

	lastNode = tail;
	memory->next = nullptr;
	lastNode->next = memory;
	tail = memory;

	// m_mutextPush;
}