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

		static Vector2Int operator+(Vector2Int a, Vector2Int b)
		{
			return Vector2Int(a.x + b.x, a.y + b.y);
		}

		static Vector2Int operator -(Vector2Int a, Vector2Int b)
		{
			return Vector2Int(a.x - b.x, a.y - b.y);
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

	class MapData
	{
	public : 
		int MinX;
		int MaxX;
		int MinY;
		int MaxY;

		int GetSizeX() const { return MaxX - MinX + 1; }
		int GetSizeY() const { return MaxY - MinY + 1; }

		bool CanGo(Vector2Int cellPos, bool checkObjects = true);
		class GameObject* Find(Vector2Int cellPos);
		bool ApplyLeave(GameObject* gameObject);
		bool ApplyMove(GameObject* gameObject, Vector2Int dest);
		void LoadMap(int mapId, string pathPrefix = "../../../../../Common/MapData");

		// A 스타 만들꺼임

	private : 
		bool** _collision;
		class GameObject*** _objects;
	};
}
