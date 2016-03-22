#pragma once
#include "stdafx.h"
enum class PacketType : unsigned char;

class PacketProcess
{
private:
	PacketProcess();
	~PacketProcess();
public:
	static PacketProcess* getInstance(){
		static PacketProcess instance;
		return &instance;
	}

	unordered_map<PacketType, function<void(unsigned int, char*)>> m_mapPacketRoutine;
	void funcRegisterd();
	bool packetProcess(const unsigned int& objID, char* buf);

	void playerLogin(unsigned int objID, char* buf);
	void playerMove(unsigned int objID, char* buf);
};