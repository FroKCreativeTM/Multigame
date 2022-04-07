#include "pch.h"
#include "Monster.h"

namespace FrokEngine
{

	Monster::Monster()

	{
		_objectType = Protocol::GameObjectType::MONSTER;

		// TEMP
		_statInfo.set_level(1);
		_statInfo.set_hp(100);
		_statInfo.set_maxhp(100);
		_statInfo.set_speed(5.0f);

		_state = Protocol::CreatureState::IDLE;
	}

	void Monster::Update()
	{
		switch (_state)
		{
		case Protocol::CreatureState::IDLE:
			UpdateIdle();
			break;
		case Protocol::CreatureState::MOVING:
			UpdateMoving();
			break;
		case Protocol::CreatureState::SKILL:
			UpdateSkill();
			break;
		case Protocol::CreatureState::DEAD:
			UpdateDead();
			break;
		}
	}

	void Monster::UpdateIdle()
	{
		if (_nextSearchTick > GetTickCount64())
			return;
		_nextSearchTick = Environment.TickCount64 + 1000;

		Player target = Room.FindPlayer(p = >
		{
			Vector2Int dir = p.CellPos - CellPos;
			return dir.cellDistFromZero <= _searchCellDist;
		});

		if (target == null)
			return;

		_target = target;
		State = CreatureState.Moving;
	}

	void Monster::UpdateMoving()
	{
		if (_nextMoveTick > Environment.TickCount64)
			return;
		int moveTick = (int)(1000 / Speed);
		_nextMoveTick = Environment.TickCount64 + moveTick;

		if (_target == null || _target.Room != Room)
		{
			_target = null;
			State = CreatureState.Idle;
			BroadcastMove();
			return;
		}

		Vector2Int dir = _target.CellPos - CellPos;
		int dist = dir.cellDistFromZero;
		if (dist == 0 || dist > _chaseCellDist)
		{
			_target = null;
			State = CreatureState.Idle;
			BroadcastMove();
			return;
		}

		List<Vector2Int> path = Room.Map.FindPath(CellPos, _target.CellPos, checkObjects: false);
		if (path.Count < 2 || path.Count > _chaseCellDist)
		{
			_target = null;
			State = CreatureState.Idle;
			BroadcastMove();
			return;
		}

		// 스킬로 넘어갈지 체크
		if (dist <= _skillRange && (dir.x == 0 || dir.y == 0))
		{
			_coolTick = 0;
			State = CreatureState.Skill;
			return;
		}

		// 이동
		Dir = GetDirFromVec(path[1] - CellPos);
		Room.Map.ApplyMove(this, path[1]);
		BroadcastMove();
	}

	void Monster::UpdateSkill()
	{
		if (_coolTick == 0)
		{
			// 유효한 타겟인지
			if (_target == null || _target.Room != Room || _target.Hp == 0)
			{
				_target = null;
				State = CreatureState.Moving;
				BroadcastMove();
				return;
			}

			// 스킬이 아직 사용 가능한지
			Vector2Int dir = (_target.CellPos - CellPos);
			int dist = dir.cellDistFromZero;
			bool canUseSkill = (dist <= _skillRange && (dir.x == 0 || dir.y == 0));
			if (canUseSkill == false)
			{
				State = CreatureState.Moving;
				BroadcastMove();
				return;
			}

			// 타게팅 방향 주시
			MoveDir lookDir = GetDirFromVec(dir);
			if (Dir != lookDir)
			{
				Dir = lookDir;
				BroadcastMove();
			}

			Skill skillData = null;
			DataManager.SkillDict.TryGetValue(1, out skillData);

			// 데미지 판정
			_target.OnDamaged(this, skillData.damage + Stat.Attack);

			// 스킬 사용 Broadcast
			S_Skill skill = new S_Skill(){ Info = new SkillInfo() };
			skill.ObjectId = Id;
			skill.Info.SkillId = skillData.id;
			Room.Broadcast(skill);

			// 스킬 쿨타임 적용
			int coolTick = (int)(1000 * skillData.cooldown);
			_coolTick = Environment.TickCount64 + coolTick;
		}

		if (_coolTick > Environment.TickCount64)
			return;

		_coolTick = 0;
	}

	void Monster::UpdateDead()
	{
	}

	void Monster::BroadcastMove()
	{
		// 다른 플레이어한테도 알려준다
		S_Move movePacket = new S_Move();
		movePacket.ObjectId = Id;
		movePacket.PosInfo = PosInfo;
		Room.Broadcast(movePacket);
	}

}