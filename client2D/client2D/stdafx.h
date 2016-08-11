// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#pragma comment(lib, "ws2_32")

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
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

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
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