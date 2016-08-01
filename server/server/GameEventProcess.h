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
	bool operator()(const GameEvent& lhs, const GameEvent& rhs){
		return rhs.wakeTime < lhs.wakeTime;
	}
};

class GameEventProcess
{
private:
	GameEventProcess(boost::asio::io_service& io_service);
	~GameEventProcess();
	boost::asio::io_service& m_io_service;
	boost::thread* m_pEventThread;
	boost::mutex* m_pLock;  //lock ÇÊ¿äÇÔ

	void lock(){ m_pLock->lock(); }
	void unlock(){ m_pLock->unlock(); }
public:
	static GameEventProcess* getInstance(boost::asio::io_service& io_service){
		static GameEventProcess instance(io_service);
		return &instance;
	}

	priority_queue<GameEvent, vector<GameEvent>, Comparison> m_EventQueue;

	unordered_map<EventType, function<void(unsigned int, GameEvent)>> m_mapEventRoutine;

	void eventThread();
	void addGameEvent(const unsigned int objID, float delayTime_ms, const EventType& type);
	void funcRegisterd();
	void eventProcess(/*const DWORD& objID, OVER_EX* overlapped*/);
	void eventToWorkerthread(const GameEvent& myEvent);
	
	void characterMove(unsigned int objID, GameEvent& gEvent);
};

