#include "stdafx.h"
#include "GameServer.h"


GameServer::GameServer()
{
}


GameServer::~GameServer()
{
}

void GameServer::initServer(){
	//���� ������ �� �� �ʿ��� �ʱ�ȭ �۾����� ����
	//�̱��� ��ü�� ����, �ʿ��� �޸𸮵� �̸� �Ҵ� ����
}

void GameServer::startServer(){
	//��Ʈ��ũ ���� ��ü �ʱ�ȭ / ����
}

void GameServer::loopServer(){
	//���� ����->���� �ֱ⸶�� �̺�Ʈ�� ó����
	/*GameEvent myEvent;
	while (true){
		Sleep(1);
		pEvent->enterCriticalSection();
		while (!pEvent->m_EventQueue.empty()){
			myEvent = pEvent->m_EventQueue.top();
			if (GetTickCount() < myEvent.wakeTime){
				pEvent->leaveCriticalSection();
				break;
			}
			pEvent->m_EventQueue.pop();
			pEvent->eventToWorkerthread(myEvent);
			if (pEvent->m_EventQueue.empty()){
				pEvent->leaveCriticalSection();
				break;
			}
		}
		pEvent->leaveCriticalSection();
	}*/
}

void GameServer::endServer(){
	//���� ���� ����
	exit(1);
}