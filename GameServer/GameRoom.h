#pragma once

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

namespace FrokEngine
{
	class GameRoom : public JobQueue
	{
	public:
		void Init(int mapId);
		void Update();
		void EnterGame(GameObjectRef gameObject);
		void LeaveGame(int32 id);
		void HandleMove(PlayerRef player, Protocol::C_MOVE movePacket);
		void HandleSkill(PlayerRef player, Protocol::C_SKILL skillPacket);
		void Broadcast(SendBufferRef packet); // 게임 세션 매니저의 Broadcast를 이용하자

		PlayerRef FindPlayer(function<int(GameObjectRef, bool b)>& func);

		class MapData* GetMapData() const;

	public : 
		void SetRoomId(int32 id) { _roomId = id; }
		int32 GetRoomId() const { return _roomId; }

	private:
		int32 _roomId;

		class MapData* _map;

		std::map<int32, PlayerRef> _players;
		std::map<int32, MonsterRef> _monsters;
		std::map<int32, ProjectileRef> _projectiles;
	};
}

extern shared_ptr<GameRoom> GRoom;