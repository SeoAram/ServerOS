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

bool PacketProcess::packetProcess(const DWORD& objID, char* buf){
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
	/*PlayerPacket* pPack = (PlayerPacket*)buf;
	PlayerInfoManager* pManager = PlayerInfoManager::getInstance();
	cout << objID << " :: Move Packet Process" << endl;
	pManager->m_playerVector[objID]->m_pCharacter->m_x = pPack->x;
	pManager->m_playerVector[objID]->m_pCharacter->m_y = pPack->y;
	pManager->m_playerVector[objID]->m_pCharacter->m_z = pPack->z;
	pManager->m_playerVector[objID]->m_pCharacter->m_dx = pPack->dx;
	pManager->m_playerVector[objID]->m_pCharacter->m_dy = pPack->dy;
	pManager->m_playerVector[objID]->m_pCharacter->m_dz = pPack->dz;
	pManager->m_playerVector[objID]->m_pCharacter->direct = pPack->direct;

	int rId = pManager->m_playerVector[objID]->roomId;
	RobbyManager* rManager = RobbyManager::getInstance();
	for (auto a : rManager->m_robbyVector[rId]->m_playerIdVec){
		NetworkEngine::getInstance()->sendPacket(a, (char*)pPack, sizeof(PlayerPacket));
	}*/
}