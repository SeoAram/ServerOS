// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#pragma comment(lib, "ws2_32")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <winsock2.h>

#include <cmath>

#include <vector>
#include <iostream>
#include <algorithm>

#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>


using namespace std;

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
#include "../../common/iniRead.h"
#include "../../common/protocol.h"
#include "../../common/PointVector3D.h"


#include "../../commonClient/GameObject.h"
#include "../../commonClient/ClientInfo.h"
#include "../../commonClient/ClientInfoManager.h"
#include "../../commonClient/GameNetwork.h"

#include "ImageResource.h"
#include "GamePlayer.h"

#include "ClientInfo2D.h"
#include "Resource.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif