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

using namespace std;

#define RADIAN 3.14159265358979323846 / 180.0
#define TURN 5

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include "../../common/iniRead.h"
#include "../../common/protocol.h"
#include "../../common/PointVector3D.h"


#include "GameObject.h"

#ifdef _DEBUG
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console")
#endif