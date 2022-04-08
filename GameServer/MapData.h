#pragma once

#include <cmath>

namespace FrokEngine
{
	struct Pos
	{
		Pos(int y, int x) { Y = y; X = x; }
		int Y;
		int X;
	};

	struct PQNode
	{
		int F;
		int G;
		int Y;
		int X;

		int CompareTo(PQNode other)
		{
			if (F == other.F)
				return 0;
			return F < other.F ? 1 : -1;
		}
	};

	struct Vector2Int
	{
		int x;
		int y;

		Vector2Int(int x, int y) { x = x; y = y; }

		static Vector2Int up() { return Vector2Int(0, 1); }
		static Vector2Int down() { return Vector2Int(0, -1); }
		static Vector2Int left() { return Vector2Int(-1, 0); }
		static Vector2Int right() { return Vector2Int(1, 0); }

		Vector2Int operator+(const Vector2Int& b)
		{
			return Vector2Int(x + b.x, y + b.y);
		}

		Vector2Int operator-(Vector2Int b)
		{
			return Vector2Int(x - b.x, y - b.y);
		}

		float magnitude() { return (float)sqrt(sqrMagnitude()); }
		int sqrMagnitude() { return (x * x + y * y);} 
		int cellDistFromZero() { return abs(x) + abs(y);} 

		Vector2Int operator+=(const Vector2Int pos)
		{
			x += pos.x;
			y += pos.y;
		}
	};

	class MapData : enable_shared_from_this<MapData>
	{
	public : 
		int MinX;
		int MaxX;
		int MinY;
		int MaxY;

		int GetSizeX() const { return MaxX - MinX + 1; }
		int GetSizeY() const { return MaxY - MinY + 1; }

		bool CanGo(Vector2Int cellPos, bool checkObjects = true);
		GameObjectRef Find(Vector2Int cellPos);
		bool ApplyLeave(GameObjectRef gameObject);
		bool ApplyMove(GameObjectRef gameObject, Vector2Int dest);
		void LoadMap(int mapId, string pathPrefix = "../../../../../Common/MapData");

		// A 스타 만들꺼임
		list<Vector2Int> FindPath(Vector2Int startCellPos, Vector2Int destCellPos, bool checkObjects = true)
		{
			list<Pos> path;

			// 점수 매기기
			// F = G + H
			// F = 최종 점수 (작을 수록 좋으며, 경로에 따라 달라진다.)
			// G = 시작점에서 해당 좌표까지 이동하는데 드는 비용(이것도 작을수록 좋음)
			// H = 목적지에서 얼마나 가까운지 (암튼 작을 수록 좋고, 고정된 값이다.)

			bool** closed = new bool*[GetSizeY()];
			for (int y = 0; y < GetSizeY(); ++y)
			{
				closed[y] = new bool[GetSizeX()];
			}



		}

	private : 
		const int _deltaY[4] = { 1, -1, 0, 0 };
		const int _deltaX[4] = { 0, 0, -1, 1 };
		const int _cost[4] = { 10, 10, 10, 10 };

	private : 
		bool** _collision;
		GameObjectRef** _objects;
	};
}
