#pragma once

#define WIN32_LEAN_AND_MEAN // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

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
#include <DBConnectionPool.h>	// DB�� ������ �Ѱ��ϴ� Pool �Ŵ��� Ŭ����

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