#include "pch.h"
#include "RoomManager.h"

namespace FrokEngine
{
	RoomManager* RoomManager::_inst = nullptr;

	shared_ptr<GameRoom> RoomManager::Add(int mapId)
	{
		shared_ptr<GameRoom> gameRoom = make_shared<GameRoom>();

		WRITE_LOCK;
		gameRoom->Init(mapId);
		gameRoom->SetRoomId(_roomId);
		_rooms[gameRoom->GetRoomId()] = gameRoom;
		_roomId++;

		return gameRoom;
	}

	bool RoomManager::Remove(int roomId)
	{
		WRITE_LOCK;
		if (_rooms.find(roomId) != _rooms.end())
		{
			_rooms.erase(roomId);
			return true;
		}
		return false;
	}

	shared_ptr<GameRoom> RoomManager::Find(int roomId)
	{
		READ_LOCK;
		if (_rooms.find(roomId) == _rooms.end())
			return _rooms.find(roomId)->second;
		return nullptr;
	}

}