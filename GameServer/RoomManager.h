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

		GameRoom Add(int mapId);
		bool Remove(int roomId);
		GameRoom Find(int roomId);
	private:
		static RoomManager* _inst;
		std::map<int, GameRoom> _rooms;
		int _roomId = 1;
	};

}
