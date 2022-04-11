#include "pch.h"
#include "MapData.h"

#include "GameObject.h"
#include "GameRoom.h"

#include "Protocol.pb.h"
#include "Struct.pb.h"

namespace FrokEngine
{
	bool MapData::CanGo(Vector2Int cellPos, bool checkObjects)
	{
		if (cellPos.x < MinX || cellPos.x > MaxX)
			return false;
		if (cellPos.y < MinY || cellPos.y > MaxY)
			return false;

		int x = cellPos.x - MinX;
		int y = MaxY - cellPos.y;
		return !_collision[y][x] && (!checkObjects || _objects[y][x] == nullptr);
	}

	GameObjectPtr MapData::Find(Vector2Int cellPos)
	{
		if (cellPos.x < MinX || cellPos.x > MaxX)
			return nullptr;
		if (cellPos.y < MinY || cellPos.y > MaxY)
			return nullptr;

		int x = cellPos.x - MinX;
		int y = MaxY - cellPos.y;
		return _objects[y][x];
	}

	bool MapData::ApplyLeave(GameObjectPtr gameObject)
	{
		if (gameObject->GetGameRoom() == nullptr)
			return false;
		if (gameObject->GetGameRoom()->GetMapData() != this)
			return false;

		Protocol::PositionInfo posInfo = gameObject->GetPosInfo();
		if (posInfo.posx() < MinX || posInfo.posx() > MaxX)
			return false;
		if (posInfo.posy() < MinY || posInfo.posy() > MaxY)
			return false;

		{
			int x = posInfo.posx() - MinX;
			int y = MaxY - posInfo.posy();
			if (_objects[y][x] == gameObject)
				_objects[y][x] = nullptr;
		}

		return true;
	}

	bool MapData::ApplyMove(GameObjectPtr gameObject, Vector2Int dest)
	{
		ApplyLeave(gameObject);

		if (gameObject->GetGameRoom() == nullptr)
			return false;
		if (gameObject->GetGameRoom()->GetMapData() != this)
			return false;

		Protocol::PositionInfo posInfo = gameObject->GetPosInfo();
		if (CanGo(dest, true) == false)
			return false;

		{
			int x = dest.x - MinX;
			int y = MaxY - dest.y;
			_objects[y][x] = gameObject;
		}

		// 실제 좌표 이동
		posInfo.set_posx(dest.x);
		posInfo.set_posy(dest.y);
		return true;
	}

	void MapData::LoadMap(int mapId, string pathPrefix)
	{
		auto new_str = std::string(3 - min(3, to_string(mapId).length()), '0') + to_string(mapId);
		string mapName = "Map_" + new_str + ".txt";
		string fullName = pathPrefix + mapName;

		ifstream ifs(fullName);
		if (ifs)
		{
			string tmp;
			std::getline(ifs, tmp);
			MinX = stoi(tmp);
			std::getline(ifs, tmp);
			MaxX = stoi(tmp);
			std::getline(ifs, tmp);
			MinY = stoi(tmp);
			std::getline(ifs, tmp);
			MaxY = stoi(tmp);

			int xCount = MaxX - MinX + 1;
			int yCount = MaxY - MinY + 1;
			_collision = new bool* [yCount];

			for (size_t y = 0; y < yCount; y++)
			{
				_collision[y] = new bool[xCount];
			}

			_objects = new GameObject * *[yCount];

			for (size_t y = 0; y < yCount; y++)
			{
				_objects[y] = new GameObject * [xCount];
			}

			for (int y = 0; y < yCount; y++)
			{
				string line;
				std::getline(ifs, line);

				for (int x = 0; x < xCount; x++)
				{
					_collision[y][x] = (line[x] == '1' ? true : false);
				}
			}
		}
	}

}