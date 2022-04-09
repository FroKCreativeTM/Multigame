#pragma once

#include <cmath>

namespace FrokEngine
{
	struct Pos
	{
		Pos() : Y(0), X(0) {}
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

		bool operator<(const PQNode& rhs)
		{
			return (F < rhs.F) && (G < rhs.G) && (Y < rhs.Y) && (X < rhs.X);
		}
	};

	struct PQNodeCompare
	{
		constexpr bool operator()(const PQNode& a, const PQNode& b) const {
			return (a.F < b.F) && (a.G < b.G) && (a.Y < b.Y) && (a.X < b.X);
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

		Vector2Int operator-(const Vector2Int& b)
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

			return *this;
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
		GameObjectPtr Find(Vector2Int cellPos);
		bool ApplyLeave(GameObjectPtr gameObject);
		bool ApplyMove(GameObjectPtr gameObject, Vector2Int dest);
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


			// (y, x) 가는 길을 한 번이라도 발견했는지
			// 발견X => MaxValue
			// 발견O => F = G + H
			int** open = new int* [GetSizeY()]; // OpenList
			for (int y = 0; y < GetSizeY(); ++y)
			{
				open[y] = new int[GetSizeX()];
			}

			for (int y = 0; y < GetSizeY(); y++)
				for (int x = 0; x < GetSizeX(); x++)
					open[y][x] = INT_MAX;

			Pos** parent = new Pos*[GetSizeY()]; // OpenList
			for (int y = 0; y < GetSizeY(); ++y)
			{
				parent[y] = new Pos[GetSizeX()];
			}

			// 오픈리스트에 있는 정보들 중에서, 가장 좋은 후보를 빠르게 뽑아오기 위한 도구
			priority_queue<PQNode, vector<PQNode>, PQNodeCompare> pq;

			// CellPos -> ArrayPos
			Pos pos = Cell2Pos(startCellPos);
			Pos dest = Cell2Pos(destCellPos);

			// 시작점 발견 (예약 진행)
			open[pos.Y][pos.X] = 10 * (abs(dest.Y - pos.Y) + abs(dest.X - pos.X));
			PQNode tmp;
			tmp.F = 10 * (abs(dest.Y - pos.Y) + abs(dest.X - pos.X));
			tmp.G = 0;
			tmp.Y = pos.Y;
			tmp.X = pos.X;
			pq.push(tmp);
			parent[pos.Y][pos.X] = Pos(pos.Y, pos.X);


			while (pq.size() > 0)
			{
				// 제일 좋은 후보를 찾는다
				PQNode node = pq.top();
				pq.pop();

				// 동일한 좌표를 여러 경로로 찾아서, 더 빠른 경로로 인해서 이미 방문(closed)된 경우 스킵
				if (closed[node.Y][node.X])
					continue;

				// 방문한다
				closed[node.Y][node.X] = true;

				// 목적지 도착했으면 바로 종료
				if (node.Y == dest.Y && node.X == dest.X)
					break;

				// 상하좌우 등 이동할 수 있는 좌표인지 확인해서 예약(open)한다
				for (int i = 0; i < 4; i++)
				{
					Pos next = Pos(node.Y + _deltaY[i], node.X + _deltaX[i]);

					// 유효 범위를 벗어났으면 스킵
					// 벽으로 막혀서 갈 수 없으면 스킵
					if (next.Y != dest.Y || next.X != dest.X)
					{
						if (CanGo(Pos2Cell(next), checkObjects) == false) // CellPos
							continue;
					}

					// 이미 방문한 곳이면 스킵
					if (closed[next.Y, next.X])
						continue;

					// 비용 계산
					int g = 0;// node.G + _cost[i];
					int h = 10 * ((dest.Y - next.Y) * (dest.Y - next.Y) + (dest.X - next.X) * (dest.X - next.X));
					// 다른 경로에서 더 빠른 길 이미 찾았으면 스킵
					if (open[next.Y][next.X] < g + h)
						continue;

					// 예약 진행
					open[dest.Y][dest.X] = g + h;
					tmp.F = g + h;
					tmp.G = g;
					tmp.Y = next.Y;
					tmp.X = next.X;
					pq.push(tmp);
					parent[next.Y][next.X] = Pos(node.Y, node.X);
				}
			}

			return CalcCellPathFromParent(parent, dest);
		}

		list<Vector2Int> CalcCellPathFromParent(Pos** parent, Pos dest)
		{
			list<Vector2Int> cells;

			int y = dest.Y;
			int x = dest.X;
			while (parent[y][x].Y != y || parent[y][x].X != x)
			{
				cells.push_back(Pos2Cell(Pos(y, x)));
				Pos pos = parent[y][x];
				y = pos.Y;
				x = pos.X;
			}
			cells.push_back(Pos2Cell(Pos(y, x)));
			cells.reverse();

			return cells;
		}

		Pos Cell2Pos(Vector2Int cell)
		{
			// CellPos -> ArrayPos
			return Pos(MaxY - cell.y, cell.x - MinX);
		}

		Vector2Int Pos2Cell(Pos pos)
		{
			// ArrayPos -> CellPos
			return Vector2Int(pos.X + MinX, MaxY - pos.Y);
		}

	private : 
		const int _deltaY[4] = { 1, -1, 0, 0 };
		const int _deltaX[4] = { 0, 0, -1, 1 };
		const int _cost[4] = { 10, 10, 10, 10 };

	private : 
		bool** _collision;
		GameObjectPtr** _objects;
	};
}
