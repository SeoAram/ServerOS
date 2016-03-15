// client.cpp : 콘솔 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"

const char SERVER_IP[] = "127.0.0.1";

// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	//MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
	exit(1);
}

// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	printf("[%s] %s", msg, (char *)lpMsgBuf);
	LocalFree(lpMsgBuf);
}

// 사용자 정의 데이터 수신 함수
int recvn(SOCKET s, char *buf, int len, int flags)
{
	int received;
	char *ptr = buf;
	int left = len;

	while (left > 0){
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR)
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received;
		ptr += received;
	}

	return (len - left);
}


class TCP_Client
{
public:
	TCP_Client(boost::asio::io_service& io_service)
		: m_io_service(io_service),
		m_Socket(io_service),
		m_nSeqNumber(0)
	{}

	void Connect(boost::asio::ip::tcp::endpoint& endpoint)
	{
		m_Socket.async_connect(endpoint,
			boost::bind(&TCP_Client::handle_connect,
			this,
			boost::asio::placeholders::error)
			);
	}


private:
	void PostWrite()
	{
		if (m_Socket.is_open() == false)
		{
			return;
		}

		if (m_nSeqNumber > 7)
		{
			m_Socket.close();
			return;
		}


		++m_nSeqNumber;

		char szMessage[128] = { 0, };
		sprintf_s(szMessage, 128 - 1, "%d - Send Message", m_nSeqNumber);

		m_WriteMessage = szMessage;

		boost::asio::async_write(m_Socket, boost::asio::buffer(m_WriteMessage),
			boost::bind(&TCP_Client::handle_write, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
			);


		PostReceive();
	}

	void PostReceive()
	{
		memset(&m_ReceiveBuffer, '\0', sizeof(m_ReceiveBuffer));

		m_Socket.async_read_some(boost::asio::buffer(m_ReceiveBuffer),
			boost::bind(&TCP_Client::handle_receive, this,
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred)
			);

	}


	void handle_connect(const boost::system::error_code& error)
	{
		if (error)
		{
			std::cout << "connect failed : " << error.message() << std::endl;
		}
		else
		{
			std::cout << "connected" << std::endl;

			PostWrite();
		}
	}

	void handle_write(const boost::system::error_code& /*error*/, size_t /*bytes_transferred*/)
	{
	}

	void handle_receive(const boost::system::error_code& error, size_t bytes_transferred)
	{
		if (error)
		{
			if (error == boost::asio::error::eof)
			{
				std::cout << "서버와 연결이 끊어졌습니다" << std::endl;
			}
			else
			{
				std::cout << "error No: " << error.value() << " error Message: " << error.message() << std::endl;
			}
		}
		else
		{
			const std::string strRecvMessage = m_ReceiveBuffer.data();
			std::cout << "서버에서 받은 메시지: " << strRecvMessage << ", 받은 크기: " << bytes_transferred << std::endl;

			PostWrite();
		}
	}



	boost::asio::io_service& m_io_service;
	boost::asio::ip::tcp::socket m_Socket;

	int m_nSeqNumber;
	std::array<char, 128> m_ReceiveBuffer;
	std::string m_WriteMessage;
};



int main()
{/*
	boost::asio::io_service io_service;

	boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(SERVER_IP), PORT_NUMBER);

	TCP_Client client(io_service);

	client.Connect(endpoint);

	io_service.run();


	std::cout << "네트웍 접속 종료" << std::endl;

	getchar();*/

	int retval;

	// 윈속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return 1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");

	char cIPAddr[CHAR_MAX];
	u_short usPort;

	// connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	//serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	//serveraddr.sin_port = htons(SERVERPORT);

	cout << "IPAddress (xxx.xxx.xxx.xxx) : "; cin >> cIPAddr;
	cout << "Port Number : "; cin >> usPort;
	getchar();
	serveraddr.sin_addr.s_addr = inet_addr(cIPAddr);
	serveraddr.sin_port = htons(usPort);

	retval = connect(sock, (SOCKADDR *)&serveraddr, sizeof(serveraddr));
	if (retval == SOCKET_ERROR) err_quit("connect()");

	// 데이터 통신에 사용할 변수
	char buf[BUFSIZE];
	int len;

	// 서버와 데이터 통신
	while (1){
		// 데이터 입력
		printf("\n[보낼 데이터] ");
		if (fgets(buf, BUFSIZE, stdin) == NULL)
			break;

		// '\n' 문자 제거
		len = strlen(buf);
		if (buf[len - 1] == '\n')
			buf[len - 1] = '\0';
		if (strlen(buf) == 0)
			continue;

		// 데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);
		if (retval == SOCKET_ERROR){
			err_display("send()");
			break;
		}
		printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);

		// 데이터 받기
		retval = recvn(sock, buf, retval, 0);
		if (retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		else if (retval == 0)
			break;

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
		printf("[받은 데이터] %s\n", buf);
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
