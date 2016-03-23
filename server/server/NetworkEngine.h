#pragma once

class PacketProcess;
class MemoryPool;
class GameEventProcess;

class NetworkEngine
{
private:
	int m_nSeqNumber;
	boost::asio::ip::tcp::acceptor m_acceptor;			//accept
	boost::asio::io_service& m_io_service;
	boost::shared_ptr<boost::asio::io_service::work> m_spWork;

	boost::thread_group m_tGroup;				//thread pool
	boost::thread* m_accpetThread;				//accept스레드

	GameMap* m_pGameMap;
	PacketProcess* m_pProcess;			//패킷 처리를 위한 프로세스
	MemoryPool* m_pMemory;				//메모리 관리
	ClientInfo* m_pSession;
	ClientInfoManager* m_pClientInfoManager;

	NetworkEngine(boost::asio::io_service& io_service); // 생성자
public:
	~NetworkEngine();
	static NetworkEngine* getInstance(boost::asio::io_service& io_service){
		static NetworkEngine* instance = nullptr;
		if (instance == nullptr){
			instance = new NetworkEngine(io_service);
		}
		return instance;
	}

	enum MsgType{
		RECVmsg = 1,
		SENDmsg,
		EVENTmsg
	};

	void handle_accept(ClientInfo* pSession, const boost::system::error_code& error);


	void err_quit(wchar_t *msg);
	void err_display(wchar_t *msg);

	void initNetworkEngine();		//네트워크 초기화

	int acceptThread();					//Accept 스레드 함수
	int workerThread();					//워커 스레드 함수


	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred);
	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/);

	template <class F>
	void post(F f){
		m_io_service.post(f);
	}
};

