#include "stdafx.h"
#include "NetworkEngine.h"


NetworkEngine::NetworkEngine(boost::asio::io_service& io_service)
: m_acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), PORT_NUMBER)),
m_bIsAccepting(false),
m_accpetThread(nullptr),
m_pProcess(nullptr),
m_pSession(nullptr)
{
	cout << "Init Engine" << endl;
}

NetworkEngine::~NetworkEngine()
{
}


void NetworkEngine::err_quit(wchar_t *msg)
{
	//������ ����Ǵ� ��� ȣ��
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
	//���� �α׸� ȭ�鿡 ���
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
	//��Ʈ��ũ ������ �ʱ�ȭ �Ѵ�.

	for (int i = 0; i < WORKED_THREAD; ++i){
		m_workerThreadPool.push_back(new boost::thread(mem_fun(&NetworkEngine::workerThread), this));
		cout << "Workerthread Create" << endl;
	}

	m_accpetThread = new boost::thread(mem_fun(&NetworkEngine::acceptThread), this);
	cout << "Acceptthread Create" << endl;
}

int NetworkEngine::acceptThread(){
	//accept�� �۾��ϴ� thread. �̰����� ���� ������ Ŭ���̾�Ʈ�� ������ �ް� ó���Ѵ�.

	ClientInfoManager* cpManager = ClientInfoManager::getInstance();
	while (true){
		Sleep(1);
		//std::cout << "Ŭ���̾�Ʈ ���� ���....." << std::endl;

		//Ŭ���̾�Ʈ �����ؼ� ��ȯ���ִ� �κ�
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
	//accept������ ȣ��Ǵ� �Լ�
	if (!error)
	{
		std::cout << "Ŭ���̾�Ʈ ���� ���� :: " << pSession->getObject()->getObjId() << std::endl;

		pSession->PostReceive();
	}
	else{
		cout << error << endl;
	}
}

int NetworkEngine::workerThread(){
	//�۾� ������. send/recv/eventó���� �з��Ͽ� �۾� ó��

	/*int retval;
	DWORD objID;
	DWORD IOByte;
	OVER_EX* overlapped = nullptr;
	NODE* rtNode = nullptr;
	GameEventProcess* pEventManager = GameEventProcess::getInstance();
	PlayerInfoManager* pPlayerManager = PlayerInfoManager::getInstance();
	ClientInfo* pClient = pPlayerManager->m_playerVector[getConnectId()];
	while (true){
	//IOCPť���� �Ϸ�� �۾��� ������ �´�
	//ó�� �����尡 ȣ�� �� ��� ������ Ǯ�� ����Ѵ�
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
// DoAction : ���� ��Ŷ�� ó���ϰ� �ٽ� ���ú��Լ��� ȣ���Ѵ�.
// Param_1 : ��Ŷ�� ���� Ŭ���̾�Ʈ�� Obj_id
// Param_2 : ���ú��� �����Ͱ� ����ִ� OVER_Ex�� ������
// Param_3 : ���ú��� ����Ʈ ��
// TODO : �ϼ��� ��Ŷ�� ��Ŷ ó�� ��ƾ���� ó���� �ϰ� �ϼ����� ���� ��Ŷ�� ���ۿ� �����صд�.
//		  ���� ��Ŷ ó���� �������� ���� ���ú긦 ȣ���Ѵ�.

// �޾Ƽ� ó���ؾ��� ����Ʈ ���� IObyte
int rest_byte = IObyte;
// ó���ؾ� �� ���۴� overlapped�� RecvBuffer
char* buffer = overlapped->m_arRecvBuffer;
// ���� ��Ŷ�� ������� overlapped�� ��Ŷ ������
int packet_size = overlapped->m_nPacketSize;
// ���� ��Ŷ ���ۿ� ����� ��Ŷ�� ������
int received = overlapped->m_nStoredSize;
// ���� ���۸� �� ó���Ҷ����� while�� �ȿ��� ó����
while (rest_byte != 0)
{
if (packet_size == 0)// ���� ó���ؾ��� ��Ŷ�� ����� 0�̸�
{
//��Ŷ�� ����� �о
BYTE *size = reinterpret_cast<BYTE *>(buffer);
//��Ŷ�� ����� ����
packet_size = *size;
}
// �䱸�Ǵ� ������� ���� ��Ŷ�� ������� ����� ����� �� ��ŭ �䱸��
int required_size = packet_size - received;
if (rest_byte >= required_size)//��Ŷ�� ���� �����ϴ� ���� ��Ŷ�� ó����
{
memcpy(overlapped->m_arPacketbuffer + received, buffer, required_size);
// ���⼭ ��Ŷ ó�� �ϴ� �κ� �߰�
bool value = m_pProcess->packetProcess(Obj_id, overlapped->m_arPacketbuffer);
if (value == false) 	cout << "packet error" << endl;
packet_size = 0;
received = 0;
buffer += required_size;
rest_byte -= required_size;
}
else // ��Ŷ ���� �Ұ���, ���� ���� ��Ŷ�� ������
{
//���� �Ұ����� ���۸� ��Ŷ ���ۿ� ����
memcpy(overlapped->m_arPacketbuffer, buffer, IObyte);
received += rest_byte;
rest_byte = 0;
}
}
// ��Ŷ ó���� �������� ���ο� ��Ŷ ����
ZeroMemory(&(overlapped->m_Overlapped), sizeof(WSAOVERLAPPED));
overlapped->m_nOperation = RECVmsg;
overlapped->m_nPacketSize = packet_size;
overlapped->m_nStoredSize = received;
DWORD flags = 0;
int ret = WSARecv(PlayerInfoManager::getInstance()->m_playerVector[Obj_id]->getSocket(), &overlapped->m_Wsabuf, 1, NULL
, &flags, &overlapped->m_Overlapped, NULL);
if (ret == SOCKET_ERROR)
{
// WSA_IO_PENDING �۾����̹Ƿ� ������ �ƴ�
if (WSAGetLastError() != WSA_IO_PENDING)
{
err_display(L"WSARecv()");
}
}
}*/

/*void NetworkEngine::sendPacket(const int obj_id, const char* buf, const int size)
{
// DoAction : Ŭ���̾�Ʈ���� ��Ŷ�� ������
// Param_1 : ���� Ŭ���̾�Ʈ�� Obj_id
// Param_2 : ���� ������ ������
// Param_3 : ���� ������ ������
// TODO : ���� Ŭ���̾�Ʈ�� Obj_id�� �÷��̾� �Ŵ��� Ŭ�������� ã�� ��Ŷ�� ����.
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