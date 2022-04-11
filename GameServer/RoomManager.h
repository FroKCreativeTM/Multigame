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
				_inst = new RoomManager;
			}
			return _inst;
		}

		~RoomManager()
		{
			if (_inst)
				delete _inst;
			_inst = nullptr;
		}

		shared_ptr<GameRoom> Add(int mapId);
		bool Remove(int roomId);
		shared_ptr<GameRoom> Find(int roomId);
	private:
		USE_LOCK;
		static RoomManager* _inst;
		std::map<int, shared_ptr<GameRoom>> GRooms;
		int GRoomId = 1;
	};

}
