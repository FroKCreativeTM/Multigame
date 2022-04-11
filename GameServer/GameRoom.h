#pragma once

#include "MapData.h"

namespace FrokEngine
{
	class GameRoom : public JobQueue
	{
	public:
		void Init(int mapId);
		void Update();
		void EnterGame(GameObjectPtr gameObject);
		void LeaveGame(int32 id);
		void HandleMove(PlayerPtr player, Protocol::C_MOVE movePacket);
		void HandleSkill(PlayerPtr player, Protocol::C_SKILL skillPacket);
		void Broadcast(SendBufferRef packet); // 게임 세션 매니저의 Broadcast를 이용하자

		PlayerPtr FindPlayer(function<bool(GameObjectPtr)>& func);

		MapData* GetMapData() const;

	public : 
		void SetRoomId(int32 id) { _roomId = id; }
		int32 GetRoomId() const { return _roomId; }

	private:
		int32 _roomId;

		MapData* _map = new MapData();

		std::map<int32, PlayerPtr> _players;
		std::map<int32, MonsterPtr> _monsters;
		std::map<int32, ProjectilePtr> _projectiles;
	};

	extern shared_ptr<GameRoom> GRoom;
}
