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
		gameRoom->SetRoomId(GRoomId);
		GRooms[gameRoom->GetRoomId()] = gameRoom;
		GRoomId++;

		return gameRoom;
	}

	bool RoomManager::Remove(int roomId)
	{
		WRITE_LOCK;
		if (GRooms.find(roomId) != GRooms.end())
		{
			GRooms.erase(roomId);
			return true;
		}
		return false;
	}

	shared_ptr<GameRoom> RoomManager::Find(int roomId)
	{
		READ_LOCK;
		if (GRooms.find(roomId) == GRooms.end())
			return GRooms.find(roomId)->second;
		return nullptr;
	}

}