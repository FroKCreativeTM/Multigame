#pragma once

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

namespace FrokEngine
{
	class GameRoom
	{
	public:
		void Init(PacketSessionRef& session, int mapId);
		void Update();
		void EnterGame(PacketSessionRef& session, class GameObject* gameObject);
		void LeaveGame(PacketSessionRef& session, int32 id);
		void HandleMove(PacketSessionRef& session, class Player* player, Protocol::C_MOVE movePacket);
		void HandleSkill(PacketSessionRef& session, class Player* player, Protocol::C_SKILL skillPacket);
		// Player FindPlayer(Func<GameObject, bool> condition)
		void Broadcast(SendBufferRef packet); // 게임 세션 매니저의 Broadcast를 이용하자

		class MapData* GetMapData() const;

	public : 
		void SetRoomId(int32 id) { _roomId = id; }
		int32 GetRoomId() const { return _roomId; }

	private:
		int32 _roomId;

		class MapData* _map;

		std::map<int32, class Player*> _players;
		std::map<int32, class Monster*> _monsters;
		std::map<int32, class Projectile*> _projectiles;
	};


}