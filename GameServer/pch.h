#pragma once

#define WIN32_LEAN_AND_MEAN // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifdef _DEBUG
#pragma comment(lib, "ServerCore\\Debug\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Debug\\libprotobufd.lib")
#else
#pragma comment(lib, "ServerCore\\Release\\ServerCore.lib")
#pragma comment(lib, "Protobuf\\Release\\libprotobuf.lib")
#endif

#include "CorePch.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

using namespace FrokEngine;

namespace FrokEngine
{
	using GameSessionRef = shared_ptr<class GameSession>;
	using GameObjectRef = shared_ptr<class GameObject>;
	using ProjectileRef = shared_ptr<class Projectile>;
	using PlayerRef = shared_ptr<class Player>;
	using MonsterRef = shared_ptr<class Monster>;
	using ArrowRef = shared_ptr<class Arrow>;
}