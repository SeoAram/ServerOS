#include "stdafx.h"
#include "NetworkEngine.h"


NetworkEngine::NetworkEngine(boost::asio::io_service& io_service)
: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
m_bIsAccepting(false),
m_accpetThread(nullptr),
m_pProcess(nullptr),
m_pSession(nullptr),
m_pClientInfoManager(ClientInfoManager::getInstance()),
m_pGameMap(GameMap::getInstance())
{
	cout << "Init Engine" << endl;
}

NetworkEngine::~NetworkEngine()
{
}


void NetworkEngine::err_quit(wchar_t *msg)
{
	//에러로 종료되는 경우 호출
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}

void NetworkEngine::err_display(wchar_t *msg)
{
	//에러 로그를 화면에 출력
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER |
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

void NetworkEngine::initNetworkEngine(){
	//네트워크 엔진을 초기화 한다.

	for (int i = 0; i < WORKED_THREAD; ++i){
		m_workerThreadPool.push_back(new boost::thread(mem_fun(&NetworkEngine::workerThread), this));
		cout << "Workerthread Create" << endl;
	}

	m_accpetThread = new boost::thread(mem_fun(&NetworkEngine::acceptThread), this);
	cout << "Acceptthread Create" << endl;
}

int NetworkEngine::acceptThread(){
	//accept를 작업하는 thread. 이곳에서 리슨 소켓이 클라이언트의 접속을 받고 처리한다.

	ClientInfoManager* cpManager = ClientInfoManager::getInstance();
	while (true){
		Sleep(1);
		//std::cout << "클라이언트 접속 대기....." << std::endl;

		//클라이언트 접속해서 반환해주는 부분
		m_pSession = cpManager->connectClient();

		if (m_pSession != nullptr){
			//m_bIsAccepting = true;
			m_acceptor.async_accept(m_pSession->Socket(),
				boost::bind(&NetworkEngine::handle_accept,
				this,
				m_pSession,
				boost::asio::placeholders::error)
				);
		}
	}
	return 0;
}

void NetworkEngine::handle_accept(ClientInfo* pSession, const boost::system::error_code& error)
{
	//accept성공시 호출되는 함수
	if (!error)
	{
		std::cout << "클라이언트 접속 성공 :: " << pSession->getObject()->getObjId() << std::endl;

		pSession->PostReceive();
		//클라이언트 접속 성공한 경우 현재 존재하는 모든 클라이언트에게 접속 여부를 알린다.
		//모든 클라에게 알려야 하나....맵에서만 체크합시다
		m_pGameMap->insertObjId(pSession->getObject()->m_pPosition->x, pSession->getObject()->m_pPosition->z, pSession->getObject()->getObjId());
		//m_pClientInfoManager->
	}
	else{
		cout << error << endl;
	}
}

int NetworkEngine::workerThread(){
	//작업 스레드. send/recv/event처리를 분류하여 작업 처리

	/*int retval;
	DWORD objID;
	DWORD IOByte;
	OVER_EX* overlapped = nullptr;
	NODE* rtNode = nullptr;
	GameEventProcess* pEventManager = GameEventProcess::getInstance();
	PlayerInfoManager* pPlayerManager = PlayerInfoManager::getInstance();
	ClientInfo* pClient = pPlayerManager->m_playerVector[getConnectId()];
	while (true){
	//IOCP큐에서 완료된 작업을 가지고 온다
	//처음 스레드가 호출 된 경우 스레드 풀에 등록한다
	retval = GetQueuedCompletionStatus(m_iocpHandle, &IOByte, &objID, reinterpret_cast<LPOVERLAPPED*>(&overlapped), INFINITE);
	if (retval == 0 || IOByte == 0){
	if (retval == 0){
	DWORD tmp1, tmp2;
	WSAGetOverlappedResult(pPlayerManager->m_playerVector[getConnectId()]->getSocket(), &overlapped->m_Overlapped, &tmp1, FALSE, &tmp2);
	err_display(L"WSAGetOverlappedResult()\n");
	}
	RobbyManager::getInstance()->leaveRobby(objID);
	pPlayerManager->deletePlayer(objID);
	continue;
	}
	switch (overlapped->m_nOperation){
	case RECVmsg:
	recvPacket(objID, overlapped, IOByte);
	break;
	case SENDmsg:
	rtNode = reinterpret_cast<NODE*>(overlapped);
	m_pMemory->returnMemory(rtNode);
	break;
	case EVENTmsg:
	pEventManager->eventProcess(objID, overlapped);
	rtNode = reinterpret_cast<NODE*>(overlapped);
	m_pMemory->returnMemory(rtNode);
	break;
	default:
	break;
	}
	}*/
	return 0;

}
