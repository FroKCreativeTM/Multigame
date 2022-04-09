#include "pch.h"
#include "RoomManager.h"

namespace FrokEngine
{
	shared_ptr<GameRoom> RoomManager::Add(int mapId)
	{
		shared_ptr<GameRoom> gameRoom = make_shared<GameRoom>();
		// gameRoom->Push(gameRoom->Init, true);

		WRITE_LOCK;
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