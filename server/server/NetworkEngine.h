#pragma once

class PacketProcess;
class MemoryPool;
class GameEventProcess;
class PlayerInfoManager;

class NetworkEngine
{
private:
	NetworkEngine();
	~NetworkEngine();
	
	DWORD connectId;
public:
	static NetworkEngine* getInstance(){
		static NetworkEngine instance;
		return &instance;
	}

	enum MsgType{
		RECVmsg = 1,
		SENDmsg,
		EVENTmsg
	};

	HANDLE m_iocpHandle;				//IOCP핸들
	WSADATA m_Wsa;

	vector<ClientInfo*> m_arClient;		//클라이언트 관리 벡터

	vector<boost::thread*> m_workerThreadPool;	//작업 스레드
	boost::thread* m_accpetThread;				//accept스레드

	PacketProcess* m_pProcess;			//패킷 처리를 위한 프로세스
	MemoryPool* m_pMemory;				//메모리 관리

	void err_quit(wchar_t *msg);
	void err_display(wchar_t *msg);

	DWORD getConnectId(){ return connectId; }	//현재 플레이어 번호
	void addConnectId(){ ++connectId; }
	void deleteConnectId(){ --connectId; }

	void initNetworkEngine();		//네트워크 초기화

	bool createIOCP();					//IOCP생성
	bool initWinsock();			//윈속 초기화

	int acceptThread();					//Accept 스레드 함수
	int workerThread();					//워커 스레드 함수

	//send, recv작업 필요
	//void recvPacket(DWORD Obj_id, OVER_EX *overlapped, const DWORD IObyte);

	//void sendPacket(const int obj_id, const char* buf, const int size);
	//void sendPacketToViewList(const int obj_id, const char* buf);
};

