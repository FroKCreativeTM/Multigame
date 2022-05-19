#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "ServerCore\\Debug\\DBCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "ServerCore\\Release\\DBCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"

#include <DBConnection.h>
#include <DBConnectionPool.h>	// DB의 연결을 총괄하는 Pool 매니저 클래스

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

using namespace FrokEngine;

namespace FrokEngine
{
	using GameSessionRef = shared_ptr<class GameSession>;
	using GameObjectPtr = class GameObject*;
	using ProjectilePtr = class Projectile*;
	using PlayerPtr = class Player*;
	using MonsterPtr = class Monster*;
	using ArrowPtr = class Arrow*;
}