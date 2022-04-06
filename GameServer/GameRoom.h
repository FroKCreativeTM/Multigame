#pragma once


namespace FrokEngine
{
	class GameRoom
	{
	public:
		void Init(int mapId);

		class MapData* GetMapData() const;

	public : 
		void SetRoomId(int32 id) { _roomId = id; }
		int32 GetRoomId() const { return _roomId; }

	private:
		int32 _roomId;

		class MapData* _map;

		std::map<int, class Player*> _players;
		std::map<int, class Monster*> _monsters;
		std::map<int, class Projectile*> _projectiles;
	};


}