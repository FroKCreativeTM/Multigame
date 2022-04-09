#pragma once

#include "Types.h"
#include "CoreMacro.h"
#include "CoreTLS.h"
#include "CoreGlobal.h"
#include "Container.h"

#include <windows.h>
#include <iostream>
#include <cmath>
#include <list>
#include <functional>
#include <queue>
using namespace std;

#include <winsock2.h>
#include <mswsock.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#include "Lock.h"
#include "ObjectPool.h"
#include "TypeCast.h"
#include "Memory.h"
#include "SendBuffer.h"
#include "Session.h"
#include "JobQueue.h"

// MySQL
#include <MySQL/mysql/jdbc.h>

#ifdef _DEBUG
#pragma comment(lib, "MySQL/Debug/libcrypto.lib")
#pragma comment(lib, "MySQL/Debug/libssl.lib")
#pragma comment(lib, "MySQL/Debug/mysqlcppconn.lib")
#else
#pragma comment(lib, "MySQL/Release/libcrypto.lib")
#pragma comment(lib, "MySQL/Release/libssl.lib")
#pragma comment(lib, "MySQL/Release/mysqlcppconn.lib")
#endif

// JsonCPP
#include <jsoncpp/include/json/json.h>
#include <fstream>