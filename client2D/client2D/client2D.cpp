// client2D.cpp : 응용 프로그램에 대한 진입점을 정의합니다.
//

#include "stdafx.h"
#include "client2D.h"

#define MAX_LOADSTRING 100

// 전역 변수:
HINSTANCE hInst;								// 현재 인스턴스입니다.
TCHAR szTitle[MAX_LOADSTRING];					// 제목 표시줄 텍스트입니다.
TCHAR szWindowClass[MAX_LOADSTRING];			// 기본 창 클래스 이름입니다.

WSADATA wsa;
SOCKET sock;		//socket
SOCKADDR_IN serveraddr;	//socekt ip

// 이 코드 모듈에 들어 있는 함수의 정방향 선언입니다.
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);



// 소켓 함수 오류 출력 후 종료
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, (LPCTSTR)msg, MB_ICONERROR);
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


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: 여기에 코드를 입력합니다.
	MSG msg;
	HACCEL hAccelTable;

	// 전역 문자열을 초기화합니다.
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_CLIENT2D, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// 응용 프로그램 초기화를 수행합니다.
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_CLIENT2D));

	// 기본 메시지 루프입니다.
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  함수: MyRegisterClass()
//
//  목적: 창 클래스를 등록합니다.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_CLIENT2D));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_CLIENT2D);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   함수: InitInstance(HINSTANCE, int)
//
//   목적: 인스턴스 핸들을 저장하고 주 창을 만듭니다.
//
//   설명:
//
//        이 함수를 통해 인스턴스 핸들을 전역 변수에 저장하고
//        주 프로그램 창을 만든 다음 표시합니다.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // 인스턴스 핸들을 전역 변수에 저장합니다.

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, IniData::getInstance()->getData("WINDOW_WIDTH"), IniData::getInstance()->getData("WINDOW_HEIGHT"),
	   NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}


//recv thread
void serverConnectThread(){
	GameObject* obj1;
	char tmp[256];
	int retval = 0;

	while (1){
		//retval = recv(sock, (char*)&headPacket, sizeof(PacketHeader), 0);
		retval = recv(sock, tmp, sizeof(PacketHeader), 0);
		if (retval == SOCKET_ERROR){
			err_display("recv()");
			break;
		}
		else if (retval == 0){
			continue;
		}
		else{
			PacketHeader* pHeader = (PacketHeader*)&tmp;
			if (pHeader->protocol == PacketType::LOGIN_PACKET){
				retval = recv(sock, (tmp + sizeof(PacketHeader)), ((PacketHeader*)tmp)->packetSize - sizeof(PacketHeader), 0);
			}
			continue;
		}
	}
}


//
//  함수: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  목적:  주 창의 메시지를 처리합니다.
//
//  WM_COMMAND	- 응용 프로그램 메뉴를 처리합니다.
//  WM_PAINT	- 주 창을 그립니다.
//  WM_DESTROY	- 종료 메시지를 게시하고 반환합니다.
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HDC mem1dc, mem2dc;
	static HBITMAP hBit1, oldBit1, oldBit2;
	static GameObjectManager* pObjectManager;
	static ClientInfo2D* pGamePlayer;
	GameObject* playerObj;
	static IniData* iData;

	static ImageResource backImage;
	static ImageResource slimeImage;

	static int BLOCK_WIDTH, BLOCK_HEIGHT;

	static boost::thread* pRecvThread;

	static boost::asio::io_service io_service;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// 메뉴 선택을 구문 분석합니다.
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	case WM_TIMER:
		if (0 == wParam){
			boost::posix_time::time_duration diff;
			if (IniData::getInstance()->getData("GAME_OBJECT_STAT") != pGamePlayer->getObject()->m_cObjState){
				pGamePlayer->getObject()->m_cObjState = IniData::getInstance()->getData("GAME_OBJECT_MOVE");

				diff = boost::posix_time::microsec_clock::local_time() - pGamePlayer->getObject()->getLastChangeTime(); //시간 차 확인
				pGamePlayer->getObject()->move(diff.total_milliseconds() * 0.001);
			}
			std::vector<GameObject*> tmp = *pObjectManager->getObjectList();
			for (int i = 0; i < tmp.size(); ++i){
				if (IniData::getInstance()->getData("GAME_OBJECT_STAT") != tmp[i]->m_cObjState){
					diff = boost::posix_time::microsec_clock::local_time() - tmp[i]->getLastChangeTime(); //시간 차 확인
					tmp[i]->move(diff.total_milliseconds() * 0.001);
				}
			}
			InvalidateRgn(hWnd, NULL, FALSE);
		}
		if (1 == wParam){
			if (IniData::getInstance()->getData("GAME_OBJECT_STAT") != pGamePlayer->getObject()->m_cObjState){
				pGamePlayer->sendPacket(PacketType::MOVE_PACKET);
			}
		}
		break;
	case WM_PAINT:
		//CGameTimer::waitClock(WINDOW_FRAME);
		hdc = BeginPaint(hWnd, &ps);
		// TODO: 여기에 그리기 코드를 추가합니다.


		if (hBit1 == NULL){
			hBit1 = CreateCompatibleBitmap(hdc, IniData::getInstance()->getData("WINDOW_WIDTH"), IniData::getInstance()->getData("WINDOW_HEIGHT"));
		}

		mem1dc = CreateCompatibleDC(hdc);
		mem2dc = CreateCompatibleDC(mem1dc);

		oldBit1 = (HBITMAP)SelectObject(mem1dc, hBit1);
		oldBit2 = (HBITMAP)SelectObject(mem2dc, hBit1);

		backImage.RenderX2(mem1dc, mem2dc);
		playerObj = pGamePlayer->getObject();
		{
			float x = ((int)playerObj->m_pvPos->x % BLOCK_WIDTH);// * 4;
			float z = ((int)playerObj->m_pvPos->z % BLOCK_HEIGHT);// * 4;
			/*float x = (playerObj->m_pvPos->x / 16) * 4;
			float z = (playerObj->m_pvPos->z / 16) * 4;*/
			slimeImage.Render(mem1dc, mem2dc, x, z);

			std::vector<GameObject*> tmp = *pObjectManager->getObjectList();
			for (int i = 0; i < tmp.size(); ++i){
				x = ((int)tmp[i]->m_pvPos->x % BLOCK_WIDTH);// * 4;
				z = ((int)tmp[i]->m_pvPos->z % BLOCK_HEIGHT);// * 4;
				/*x = tmp[i]->m_pvPos->x / 4;
				z = tmp[i]->m_pvPos->z / 4;*/
				slimeImage.Render(mem1dc, mem2dc, x, z);
			}
		}

		SetBkMode(mem1dc, TRANSPARENT);

		BitBlt(hdc, 0, 0, IniData::getInstance()->getData("WINDOW_WIDTH"), IniData::getInstance()->getData("WINDOW_HEIGHT"), mem1dc, 0, 0, SRCCOPY);

		SelectObject(mem2dc, oldBit2); DeleteDC(mem2dc);
		SelectObject(mem1dc, oldBit1); DeleteDC(mem1dc);



		EndPaint(hWnd, &ps);
		break;
	case WM_KEYDOWN:
		if (IniData::getInstance()->getData("GAME_OBJECT_STAT") != pGamePlayer->getObject()->m_cObjState){
			if (VK_UP == wParam){
				pGamePlayer->getObject()->turnUp();
			}
			if (VK_DOWN == wParam){
				pGamePlayer->getObject()->turnDown();
			}
			if (VK_LEFT == wParam){
				pGamePlayer->getObject()->turnLeft();
			}
			if (VK_RIGHT == wParam){
				pGamePlayer->getObject()->turnRight();
			}

			pGamePlayer->sendPacket(PacketType::MOVE_PACKET);
		}
		pGamePlayer->getObject()->m_pvDir->operator<<(cout) << endl;
		break;
	case WM_DESTROY:
		pRecvThread->join();
		pGamePlayer->Socket().close();
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		
	{
					  iData = IniData::getInstance();
					  pObjectManager = GameObjectManager::getInstance();
					  char cIPAddr[CHAR_MAX];
					  u_short usPort;


					  cout << "IPAddress (xxx.xxx.xxx.xxx) : "; cin >> cIPAddr;
					  cout << "Port Number : "; cin >> usPort;
					  getchar();

					  boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::address::from_string(cIPAddr), usPort);
					  pGamePlayer = new ClientInfo2D(io_service, endpoint);
					  pRecvThread = new boost::thread(boost::bind(&boost::asio::io_service::run, &io_service));

					  SetTimer(hWnd, 0, IniData::getInstance()->getData("FRAME_RATE") / 1000, NULL);
					  SetTimer(hWnd, 1, 1000, NULL);

					  backImage.setImgResource(IMG_BACK, 1, 1, lParam, IniData::getInstance()->getData("IMAGE_SINGLE_FRAME"));
					  slimeImage.setImgResource(SLIME, 1, 1, lParam, IniData::getInstance()->getData("IMAGE_SINGLE_FRAME"), 30, 89, 148);
					  BLOCK_WIDTH = IniData::getInstance()->getData("BLOCK_SIZE");
					  BLOCK_HEIGHT = IniData::getInstance()->getData("BLOCK_SIZE");
	}
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// 정보 대화 상자의 메시지 처리기입니다.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
