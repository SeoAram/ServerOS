// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#pragma comment(lib, "ws2_32")

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>
#include <winsock2.h>

#include <iostream>
#include <queue>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/asio/steady_timer.hpp>
#include <boost/chrono.hpp>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.

#include "../../Common/protocol.h"
#include "../../Common/iniRead.h"
#include "../../Common/PointVector3D.h"

#include "../../commonClient/GameObject.h"
#include "../../commonClient/ClientInfo.h"
#include "../../commonClient/ClientInfoManager.h"
#include "../../commonClient/GameNetwork.h"


#define SERVERIP   "127.0.0.1"
#define SERVERPORT 13400
#define BUFSIZE    128

using namespace std;