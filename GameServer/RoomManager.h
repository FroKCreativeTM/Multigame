#pragma once

#include "GameRoom.h"

namespace FrokEngine
{
	class RoomManager
	{
	public:
		static shared_ptr<RoomManager> GetInst()
		{
			if (!_inst)
			{
				_inst = make_shared<RoomManager>();
			}
			return _inst;
		}

		shared_ptr<GameRoom> Add(int mapId);
		bool Remove(int roomId);
		shared_ptr<GameRoom> Find(int roomId);
	private:
		USE_LOCK;
		static shared_ptr<RoomManager> _inst;
		std::map<int, shared_ptr<GameRoom>> _rooms;
		int _roomId = 1;
	};

}
