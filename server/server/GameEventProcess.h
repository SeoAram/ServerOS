#pragma once
#include "stdafx.h"
enum class EventType : unsigned char;

struct GameEvent{
	unsigned int objID;
	boost::posix_time::ptime wakeTime;
	float delayTime_ms;
	EventType eType;
};

class Comparison{
public:
	bool operator()(const GameEvent* lhs, const GameEvent* rhs){
		return rhs->wakeTime < lhs->wakeTime;
	}
};

class GameEventProcess
{
private:
	GameEventProcess();
	~GameEventProcess();
	boost::thread* m_pEventThread;
	std::vector<boost::thread*> m_vThread;
	boost::mutex* m_pLock;  //lock 필요함

	boost::mutex* m_pMemoryLock;  //lock 필요함
	queue<GameEvent*> m_pEventMemory;

	const int MAX_EVENT_THREAD = 1;

	void memoryLock(){ m_pMemoryLock->lock(); }
	void memoryUnlock(){ m_pMemoryLock->unlock(); }
	void lock(){ m_pLock->lock(); }
	void unlock(){ m_pLock->unlock(); }
public:
	static GameEventProcess* getInstance(){
		static GameEventProcess instance;
		return &instance;
	}

	priority_queue<GameEvent, vector<GameEvent*>, Comparison> m_EventQueue;

	unordered_map<EventType, function<void(unsigned int, GameEvent*)>> m_mapEventRoutine;

	void eventThread();
	void addGameEvent(const unsigned int objID, float delayTime_ms, const EventType& type);
	void funcRegisterd();
	void eventProcess(/*const DWORD& objID, OVER_EX* overlapped*/);
	void eventToWorkerthread(const int threadNum);
	
	void characterMove(unsigned int objID, GameEvent* gEvent);
};

