#pragma once
#include "stdafx.h"
enum class EventType : unsigned char;

struct GameEvent{
	unsigned int objID;
	unsigned int wakeTime;

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
	boost::mutex m_mLock;  //lock ÇÊ¿äÇÔ
public:
	static GameEventProcess* getInstance(boost::asio::io_service& io_service){
		static GameEventProcess instance(io_service);
		return &instance;
	}

	priority_queue<GameEvent, vector<GameEvent>, Comparison> m_EventQueue;

	unordered_map<EventType, function<void(unsigned int)>> m_mapEventRoutine;

	void eventThread();
	void addGameEvent(const unsigned int objID, const unsigned int time, const EventType& type);
	void funcRegisterd();
	void eventProcess(/*const DWORD& objID, OVER_EX* overlapped*/);
	void eventToWorkerthread(const GameEvent& myEvent);
	
	void monsterMove(unsigned int objID);
};

