#include "stdafx.h"
#include "MemoryPool.h"


MemoryPool::MemoryPool()
{
	//queue형태의 메모리 구조 생성 및 Lock생성 -> Lock을 뮤텍스로 변경해야함
	InitializeCriticalSection(&m_csPopLock);
	InitializeCriticalSection(&m_csPushLock);

	head = new NODE;
	tail = head;
}


MemoryPool::~MemoryPool()
{
}


void MemoryPool::createMemoryPool(){
	//MemoryPool생성
	NODE* node;
	for (int i = 0; i < 100000; ++i){
		node = new NODE;
		tail->m_pNext = node;
		tail = node;
	}
	tail->m_pNext = nullptr;
}

NODE* MemoryPool::getMemory(){
	//queue에서 메모리 노드를 하나 가져간다
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
	//사용된 메모리를 반환받는다
	NODE* lastNode;

	EnterCriticalSection(&m_csPushLock);
	lastNode = tail;
	memory->m_pNext = nullptr;
	lastNode->m_pNext = memory;
	tail = memory;

	LeaveCriticalSection(&m_csPushLock);
}