#include "stdafx.h"
#include "PacketProcess.h"


PacketProcess::PacketProcess()
{
}


PacketProcess::~PacketProcess()
{
}

void PacketProcess::funcRegisterd(){
	//��Ŷ�� ���� ó���ؾ� �ϴ� �Լ� ���
	//m_mapPacketRoutine[PacketType::LOGIN_PACKET] = bind(&PacketProcess::playerLogin, this, placeholders::_1, placeholders::_2);
}

bool PacketProcess::packetProcess(const unsigned int& objID, char* buf){
	//��Ŷ ó��
	/*PacketHeader *packet = reinterpret_cast<PacketHeader*>(buf);
	auto iter = m_mapPacketRoutine.find(packet->protocol);
	if (iter != m_mapPacketRoutine.end()){
		m_mapPacketRoutine[packet->protocol](objID, buf);
		return true;
	}
	else{
		return false;
	}
	*/
	return false;
}

void PacketProcess::playerLogin(unsigned int objID, char* buf){
	cout << objID << " Player Login" << endl;
}

void PacketProcess::playerMove(unsigned int objID, char* buf){
	
}