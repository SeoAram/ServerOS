#pragma once

class PacketProcess;
class MemoryPool;
class GameEventProcess;

class NetworkEngine
{
private:
	int m_nSeqNumber;
	boost::asio::ip::tcp::acceptor m_acceptor;
	ClientInfo* m_pSession;
	NetworkEngine(boost::asio::io_service& io_service);
public:
	~NetworkEngine();
	static NetworkEngine* getInstance(boost::asio::io_service& io_service){
		//static bool bInit = false;
		static NetworkEngine* instance = nullptr;
		if (instance == nullptr){
			instance = new NetworkEngine(io_service);
			//static NetworkEngine instance(io_service);
			//bInit = true;
			//return &instance;
		}
		return instance;
	}

	enum MsgType{
		RECVmsg = 1,
		SENDmsg,
		EVENTmsg
	};

	vector<boost::thread*> m_workerThreadPool;	//작업 스레드
	boost::thread* m_accpetThread;				//accept스레드

	void handle_accept(ClientInfo* pSession, const boost::system::error_code& error);

	PacketProcess* m_pProcess;			//패킷 처리를 위한 프로세스
	MemoryPool* m_pMemory;				//메모리 관리

	void err_quit(wchar_t *msg);
	void err_display(wchar_t *msg);

	void initNetworkEngine();		//네트워크 초기화

	int acceptThread();					//Accept 스레드 함수
	int workerThread();					//워커 스레드 함수
};

