#include "pch.h"
#include "Monster.h"

#include "ClientPacketHandler.h"

#include "DataManager.h"

#include "GameRoom.h"
#include "Player.h"

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
		_nextSearchTick = GetTickCount64() + 1000;

		// �� ���� ���ٸ� ��� �ѱ� ���ΰ�
		function<bool(GameObjectRef)> ref = [=](GameObjectRef p) {
			Vector2Int dir = static_pointer_cast<Player>(p)->GetCellPos() - GetCellPos();
			return dir.cellDistFromZero() <= _searchCellDist;
		};
		PlayerRef target = _room->FindPlayer(ref);

		if (target == nullptr)
			return;

		_target = target;
		_state = Protocol::CreatureState::MOVING;
	}

	void Monster::UpdateMoving()
	{
		if (_nextMoveTick > GetTickCount64())
			return;
		int moveTick = (int)(1000 / _speed);
		_nextMoveTick = GetTickCount64() + moveTick;

		if (_target == nullptr || _target->GetGameRoom() != _room)
		{
			_target = nullptr;
			_state = Protocol::CreatureState::IDLE;
			BroadcastMove();
			return;
		}

		Vector2Int dir = _target->GetCellPos() - _cellPos;
		int dist = dir.cellDistFromZero();
		if (dist == 0 || dist > _chaseCellDist)
		{
			_target = nullptr;
			_state = Protocol::CreatureState::IDLE;
			BroadcastMove();
			return;
		}

		list<Vector2Int> path = _room->GetMapData()->FindPath(_cellPos, _target->GetCellPos(), false);
		if (path.size() < 2 || path.size() > _chaseCellDist)
		{
			_target = nullptr;
			_state = Protocol::CreatureState::IDLE;
			BroadcastMove();
			return;
		}

		// ��ų�� �Ѿ�� üũ
		if (dist <= _skillRange && (dir.x == 0 || dir.y == 0))
		{
			_coolTick = 0;
			_state = Protocol::CreatureState::SKILL;
			return;
		}

		List<Vector2Int>::iterator iter = path.begin();
		std::advance(iter, 1);

		// �̵�
		_dir = GetDirFromVec(*iter - _cellPos);

		_room->GetMapData()->ApplyMove((MonsterRef)this, *iter);
		BroadcastMove();
	}

	void Monster::UpdateSkill()
	{
		if (_coolTick == 0)
		{
			// ��ȿ�� Ÿ������
			if (_target == nullptr || _target->GetGameRoom() != _room || _target->GetHP() == 0)
			{
				_target = nullptr;
				_state = Protocol::CreatureState::MOVING;
				BroadcastMove();
				return;
			}

			// ��ų�� ���� ��� ��������
			Vector2Int dir = (_target->GetCellPos() - _cellPos);
			int dist = dir.cellDistFromZero();
			bool canUseSkill = (dist <= _skillRange && (dir.x == 0 || dir.y == 0));
			if (canUseSkill == false)
			{
				_state = Protocol::CreatureState::MOVING;
				BroadcastMove();
				return;
			}

			// Ÿ���� ���� �ֽ�
			Protocol::MoveDir lookDir = GetDirFromVec(dir);
			if (_dir != lookDir)
			{
				_dir = lookDir;
				BroadcastMove();
			}

			auto skillData = DataManager::GetSkillMap().find(1);

			// ������ ����
			_target->OnDamaged((GameObjectRef)this, skillData->second->damage + _statInfo.attack());

			// ��ų ��� Broadcast
			Protocol::S_SKILL skill;
			skill.set_objectid(GetId());
			auto info = skill.mutable_info();
			info->set_skillid(skillData->first);

			auto pkt = ClientPacketHandler::MakeSendBuffer(skill);
			_room->Broadcast(pkt);

			// ��ų ��Ÿ�� ����
			int coolTick = (int)(1000 * skillData->second->cooldown);
			_coolTick = GetTickCount64() + coolTick;
		}

		if (_coolTick > GetTickCount64())
			return;

		_coolTick = 0;
	}

	void Monster::UpdateDead()
	{
	}

	void Monster::BroadcastMove()
	{
		// �ٸ� �÷��̾����׵� �˷��ش�
		Protocol::S_MOVE movePacket;
		movePacket.set_objectid(GetId());
		auto posinfo = movePacket.mutable_posinfo();
		posinfo->set_posx(_posInfo.posx());
		posinfo->set_posy(_posInfo.posy());
		posinfo->set_state(_posInfo.state());
		posinfo->set_movedir(_posInfo.movedir());

		auto pkt = ClientPacketHandler::MakeSendBuffer(movePacket);
		_room->Broadcast(pkt);
	}

}