#include "pch.h"
#include "GameRoom.h"

#include "MapData.h"

namespace FrokEngine
{
	void GameRoom::Init(int mapId)
	{
		_map->LoadMap(mapId);

		// TEMP
		Monster monster = ObjectManager.Instance.Add<Monster>();
		monster.CellPos = new Vector2Int(5, 5);
		EnterGame(monster);
	}

	MapData* GameRoom::GetMapData() const
	{
		return nullptr;
	}

}