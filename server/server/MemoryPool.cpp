#include "stdafx.h"
#include "MemoryPool.h"


MemoryPool::MemoryPool()
{
	//queue������ �޸� ���� ���� �� Lock���� -> Lock�� ���ؽ��� �����ؾ���
	InitializeCriticalSection(&m_csPopLock);
	InitializeCriticalSection(&m_csPushLock);

	head = new NODE;
	tail = head;
}


MemoryPool::~MemoryPool()
{
}


void MemoryPool::createMemoryPool(){
	//MemoryPool����
	NODE* node;
	for (int i = 0; i < 100000; ++i){
		node = new NODE;
		tail->m_pNext = node;
		tail = node;
	}
	tail->m_pNext = nullptr;
}

NODE* MemoryPool::getMemory(){
	//queue���� �޸� ��带 �ϳ� ��������
	NODE* curNode;

	EnterCriticalSection(&m_csPopLock);
	curNode = head;
	if (curNode->m_pNext == nullptr){
		LeaveCriticalSection(&m_csPopLock);
		return nullptr;
	}
	else{
		head = head->m_pNext;
		curNode->m_pNext = nullptr;
		LeaveCriticalSection(&m_csPopLock);
		return curNode;
	}
	LeaveCriticalSection(&m_csPopLock);
	return nullptr;
}

void MemoryPool::returnMemory(NODE* memory){
	//���� �޸𸮸� ��ȯ�޴´�
	NODE* lastNode;

	EnterCriticalSection(&m_csPushLock);
	lastNode = tail;
	memory->m_pNext = nullptr;
	lastNode->m_pNext = memory;
	tail = memory;

	LeaveCriticalSection(&m_csPushLock);
}