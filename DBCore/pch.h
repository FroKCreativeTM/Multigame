﻿#pragma once

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "MySQL\\Debug\\mysqlcppconn.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "MySQL\\Release\\mysqlcppconn.lib")
#endif

// MySQL
#include "CorePch.h"