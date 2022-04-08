#pragma once

#include "GameRoom.h"

namespace FrokEngine
{
	class RoomManager
	{
	public:
		static RoomManager* GetInst()
		{
			if (!_inst)
			{
				_inst = new RoomManager();
			}
			return _inst;
		}

		shared_ptr<GameRoom> Add(int mapId);
		bool Remove(int roomId);
		shared_ptr<GameRoom> Find(int roomId);
	private:
		USE_LOCK;
		static RoomManager* _inst;
		std::map<int, shared_ptr<GameRoom>> _rooms;
		int _roomId = 1;
	};

}
