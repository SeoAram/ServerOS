#include "stdafx.h"
#include "NetworkEngine.h"


NetworkEngine::NetworkEngine() :
m_accpetThread(nullptr),
m_pProcess(nullptr)
{
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
	// asio생성
	//memorypool / packetprocess초기화
	//threadpool에 workerthread생성
	/*if (!initWinsock()) return;
	cout << "Winsock init" << endl;

	if (!createIOCP()) return;
	cout << "IOCP handle create" << endl;

	for (int i = 0; i < MAX_THREAD; ++i){
		m_workerThreadPool.push_back(new thread(mem_fun(&NetworkEngine::workerThread), this));
		cout << "Workerthread Create" << endl;
	}

	m_accpetThread = new thread(mem_fun(&NetworkEngine::acceptThread), this);
	cout << "Acceptthread Create" << endl;

	m_pMemory = MemoryPool::getInstance();
	m_pProcess = PacketProcess::getInstance();*/
}

int NetworkEngine::acceptThread(){
	//accept를 작업하는 thread. 이곳에서 리슨 소켓이 클라이언트의 접속을 받고 처리한다.
	/*int retval;

	PlayerInfoManager* pPlayerManager = PlayerInfoManager::getInstance();
	ClientInfo* pClient = pPlayerManager->m_playerVector[getConnectId()];

	// socket()
	SOCKET listen_sock = WSASocket(AF_INET, SOCK_STREAM, IPPROTO_TCP, NULL, 0, WSA_FLAG_OVERLAPPED);
	if (listen_sock == INVALID_SOCKET) err_quit(L"socket()");

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_port = htons(SERVERPORT);
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind()
	retval = ::bind(listen_sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit(L"bind()");

	// listen()
	retval = listen(listen_sock, SOMAXCONN);
	if (retval == SOCKET_ERROR) err_quit(L"listen()");

	SOCKADDR_IN clientaddr;
	int addrlen = sizeof(clientaddr);

	while (1){
		// 클라이언트 접속 받기 및 대기
		cout << "Curent Accept Count : " << getConnectId() << endl;

		SOCKET clientSock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		if (clientSock == INVALID_SOCKET){
			err_display(L"Accept()");
			continue;
		}
		pPlayerManager->insertPlayer(clientSock, getConnectId());

		ZeroMemory(&pClient->m_stOver.m_Overlapped, sizeof(WSAOVERLAPPED));
		pClient->m_stOver.m_nOperation = RECVmsg;
		pClient->m_stOver.m_nPacketSize = 0;
		pClient->m_stOver.m_nStoredSize = 0;
		//Client와 IOCP객체 연결
		CreateIoCompletionPort((HANDLE)clientSock, m_iocpHandle, getConnectId(), 0);
		DWORD flags = 0;
		//최초 데이터 전송 작업 필요
		retval = WSARecv(clientSock, &pClient->m_stOver.m_Wsabuf, 1, NULL, &flags, &pClient->m_stOver.m_Overlapped, NULL);

		if (retval == SOCKET_ERROR){
			if (WSAGetLastError() != ERROR_IO_PENDING){
				pPlayerManager->deletePlayer(getConnectId());
				continue;
			}
		}
		pClient->setOpenClient();

		PlayerPacket pPack;
		pPack.id = getConnectId();
		pPack.dx = pPack.dy = pPack.dz = pPack.x = pPack.y = pPack.z = 0;
		pPack.packetSize = sizeof(pPack);
		pPack.protocol = PacketType::PLAYER_PACKET;
		sendPacket(getConnectId(), (const char*)&pPack, sizeof(pPack));

		sendPacket(getConnectId(), (const char*)&(RobbyManager::getInstance()->m_listPack), sizeof(RoomListPacket));

		addConnectId();

	}*/
	return 0;
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

/*void NetworkEngine::recvPacket(DWORD Obj_id, OVER_EX *overlapped, const DWORD IObyte)
{
	//PlayerInfoManager* pManager = PlayerInfoManager::getInstance();
	// DoAction : 받은 패킷을 처리하고 다시 리시브함수를 호출한다.
	// Param_1 : 패킷을 보낸 클라이언트의 Obj_id
	// Param_2 : 리시브한 데이터가 담겨있는 OVER_Ex의 포인터
	// Param_3 : 리시브한 바이트 수
	// TODO : 완성된 패킷은 패킷 처리 루틴으로 처리를 하고 완성되지 않은 패킷은 버퍼에 저장해둔다.
	//		  받은 패킷 처리가 끝났으면 새로 리시브를 호출한다.
	
	// 받아서 처리해야할 바이트 수는 IObyte
	int rest_byte = IObyte;
	// 처리해야 할 버퍼는 overlapped의 RecvBuffer
	char* buffer = overlapped->m_arRecvBuffer;
	// 받은 패킷의 사이즈는 overlapped의 패킷 사이즈
	int packet_size = overlapped->m_nPacketSize;
	// 현재 패킷 버퍼에 저장된 패킷의 사이즈
	int received = overlapped->m_nStoredSize;

	// 받은 버퍼를 다 처리할때까지 while문 안에서 처리함
	while (rest_byte != 0)
	{
		if (packet_size == 0)// 현재 처리해야할 패킷의 사이즈가 0이면
		{
			//패킷의 사이즈를 읽어서
			BYTE *size = reinterpret_cast<BYTE *>(buffer);
			//패킷의 사이즈를 지정
			packet_size = *size;
		}
		// 요구되는 사이즈는 현재 패킷의 사이즈에서 저장된 사이즈를 뺀 만큼 요구됨
		int required_size = packet_size - received;

		if (rest_byte >= required_size)//패킷이 조립 가능하니 받은 패킷을 처리함
		{
			memcpy(overlapped->m_arPacketbuffer + received, buffer, required_size);
			// 여기서 패킷 처리 하는 부분 추가
			bool value = m_pProcess->packetProcess(Obj_id, overlapped->m_arPacketbuffer);
			if (value == false) 	cout << "packet error" << endl;
			packet_size = 0;
			received = 0;
			buffer += required_size;
			rest_byte -= required_size;
		}
		else // 패킷 조립 불가능, 수신 덜된 패킷이 존재함
		{
			//조립 불가능한 버퍼를 패킷 버퍼에 저장
			memcpy(overlapped->m_arPacketbuffer, buffer, IObyte);
			received += rest_byte;
			rest_byte = 0;
		}
	}
	// 패킷 처리가 끝났으면 새로운 패킷 수신
	ZeroMemory(&(overlapped->m_Overlapped), sizeof(WSAOVERLAPPED));
	overlapped->m_nOperation = RECVmsg;
	overlapped->m_nPacketSize = packet_size;
	overlapped->m_nStoredSize = received;
	DWORD flags = 0;
	int ret = WSARecv(PlayerInfoManager::getInstance()->m_playerVector[Obj_id]->getSocket(), &overlapped->m_Wsabuf, 1, NULL
		, &flags, &overlapped->m_Overlapped, NULL);
	if (ret == SOCKET_ERROR)
	{
		// WSA_IO_PENDING 작업중이므로 오류가 아님
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			err_display(L"WSARecv()");
		}
	}
}*/

/*void NetworkEngine::sendPacket(const int obj_id, const char* buf, const int size)
{
	// DoAction : 클라이언트에게 패킷을 보낸다
	// Param_1 : 받을 클라이언트의 Obj_id
	// Param_2 : 보낼 버퍼의 포인터
	// Param_3 : 보낼 버퍼의 사이즈
	// TODO : 보낼 클라이언트의 Obj_id를 플레이어 매니저 클래스에서 찾아 패킷을 보냄.
	PlayerInfoManager *pManager = PlayerInfoManager::getInstance();
	ClientInfo *Client = pManager->m_playerVector[obj_id];
	if (Client == nullptr) return;

	NODE *Send = m_pMemory->getMemory();
	Send->m_stBuf.m_nOperation = SENDmsg;
	ZeroMemory(&Send->m_stBuf.m_Overlapped, sizeof(WSAOVERLAPPED));
	Send->m_stBuf.m_WsaBuf.buf = Send->m_stBuf.m_arPacketBuf;
	Send->m_stBuf.m_WsaBuf.len = size;
	memcpy(Send->m_stBuf.m_arPacketBuf, buf, size);
	DWORD SendByte;
	int ret = WSASend(Client->getSocket(), &Send->m_stBuf.m_WsaBuf, 1,
		&SendByte, 0, &Send->m_stBuf.m_Overlapped, NULL);
	
}*/
/*void NetworkEngine::sendPacketToViewList(const int obj_id, const char* buf)
{

}*/