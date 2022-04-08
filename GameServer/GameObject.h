#pragma once

#include "Struct.pb.h"
#include "MapData.h"

namespace FrokEngine
{
	class GameObject
	{
	public :
		GameObject();
		~GameObject();

	public : 

	public : 
		Protocol::StatInfo GetStat() { return _statInfo; }
		void SetStat(const Protocol::StatInfo& stat) { _statInfo = stat; }

		Protocol::PositionInfo GetPosInfo() { return _posInfo; }
		void SetPosInfo(const Protocol::PositionInfo& posInfo) { _posInfo = posInfo; }

		Protocol::GameObjectType GetGameObjectType() const { return _objectType; }
		void SetGameObjectType(const Protocol::GameObjectType& objectType) { _objectType = objectType; }

		Protocol::ObjectInfo GetObjectInfo() { return _objInfo; }
		void SetObjectInfo(const Protocol::ObjectInfo& objInfo) { _objInfo = objInfo; }

		void SetId(int id) { _objInfo.set_objectid(id); }
		int32 GetId() const { return _objInfo.objectid(); }
		
		shared_ptr<class GameRoom> GetGameRoom() const;
		void SetGameRoom(shared_ptr<class GameRoom> room);

		float GetSpeed() const { return _speed; }
		void SetSpeed(float speed) { _speed = speed; }

		int32 GetHP() const { return _hp; }
		void SetHP(int32 hp) { _hp = hp; }

		Protocol::MoveDir GetDir() const { return _dir; }
		void SetDir(const Protocol::MoveDir& dir) { _dir = dir; }

		Protocol::CreatureState GetCreatureState() const { return _state; }
		void SetCreatureState(const Protocol::CreatureState& state) { _state = state; }

		Vector2Int GetCellPos() { return Vector2Int(_posInfo.posx(), _posInfo.posy()); }
		void SetCellPos(int32 x, int32 y) { _posInfo.set_posx(x); _posInfo.set_posy(y); }
		
		Vector2Int GetFrontCellPos()
		{
			return GetFrontCellPos(_posInfo.movedir());
		}

		Vector2Int GetFrontCellPos(Protocol::MoveDir dir)
		{
			Vector2Int cellPos = _cellPos;

			switch (dir)
			{
			case Protocol::MoveDir::UP:
				cellPos += Vector2Int::up();
				break;
			case Protocol::MoveDir::DOWN:
				cellPos += Vector2Int::down();
				break;
			case Protocol::MoveDir::LEFT:
				cellPos += Vector2Int::left();
				break;
			case Protocol::MoveDir::RIGHT:
				cellPos += Vector2Int::right();
				break;
			}

			return cellPos;
		}

		static Protocol::MoveDir GetDirFromVec(Vector2Int dir)
		{
			if (dir.x > 0)
				return Protocol::MoveDir::RIGHT;
			else if (dir.x < 0)
				return Protocol::MoveDir::LEFT;
			else if (dir.y > 0)
				return Protocol::MoveDir::UP;
			else
				return Protocol::MoveDir::DOWN;
		}

		virtual void OnDamaged(GameObjectRef attacker, int damage);
		virtual void OnDead(GameObjectRef attacker);

	public : 
		virtual void Update();

	protected : 
		Protocol::ObjectInfo			_objInfo;
		Protocol::PositionInfo			_posInfo;
		Protocol::StatInfo				_statInfo;
		Protocol::GameObjectType		_objectType;
		shared_ptr<GameRoom>			_room;
		float							_speed;
		int32							_hp;
		Protocol::MoveDir				_dir;
		Protocol::CreatureState			_state;
		Vector2Int						_cellPos;
	};
}