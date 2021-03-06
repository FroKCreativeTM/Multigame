#include "pch.h"
#include "GameRoom.h"

#include "ClientPacketHandler.h"

#include "MapData.h"
#include "DataManager.h"

#include "Player.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "Projectile.h"
#include "Arrow.h"

namespace FrokEngine
{
	shared_ptr<GameRoom> GRoom = make_shared<GameRoom>();
	void GameRoom::Init(int mapId)
	{
		_map->LoadMap(mapId);

		// TEMP
		MonsterPtr monster = ObjectManager::GetInst()->Add<Monster>();
		monster->SetCellPos(5, 5);
		EnterGame(dynamic_cast<GameObjectPtr>(monster));
	}

	void GameRoom::Update()
	{
		for(auto monster : _monsters)
		{
			monster.second->Update();
		}

		for(auto projectile : _projectiles)
		{
			projectile.second->Update();
		}

		// Flush(); // job 이걸 어떻게 처리할까 고민중
	}

	void GameRoom::EnterGame(GameObjectPtr gameObject)
	{
		if (gameObject == nullptr)
			return;

		Protocol::GameObjectType type = ObjectManager::GetInst()->GetObjectTypeById(gameObject->GetId());

		if (type == Protocol::GameObjectType::PLAYER)
		{
			PlayerPtr player = dynamic_cast<PlayerPtr>(gameObject);
			_players[gameObject->GetId()] = player;

			player->Room = this;

			_map->ApplyMove(player, Vector2Int(player->GetCellPos().x, player->GetCellPos().y));

			// 본인한테 정보 전송
			{
				Protocol::S_ENTERGAME enterPacket;

				auto p = enterPacket.mutable_player();
				p->set_name("Player_" + to_string(player->GetObjectInfo().objectid()));
				p->set_objectid(player->GetObjectInfo().objectid());

				auto pos = p->mutable_posinfo();
				pos->set_posx(player->GetPosInfo().posx());
				pos->set_posy(player->GetPosInfo().posy());
				pos->set_state(player->GetPosInfo().state());
				pos->set_movedir(player->GetPosInfo().movedir());

				auto stat = p->mutable_statinfo();
				stat->set_attack(player->GetStat().attack());
				stat->set_hp(player->GetStat().hp());
				stat->set_level(player->GetStat().level());
				stat->set_maxhp(player->GetStat().maxhp());
				stat->set_speed(player->GetStat().speed());
				stat->set_totalexp(player->GetStat().totalexp());

				auto info = enterPacket.mutable_player();
				info->set_objectid(player->GetObjectInfo().objectid());
				info->set_name("Player_" + to_string(player->GetObjectInfo().objectid()));

				pos = info->mutable_posinfo();
				pos->set_posx(player->GetPosInfo().posx());
				pos->set_posy(player->GetPosInfo().posy());
				pos->set_state(player->GetPosInfo().state());
				pos->set_movedir(player->GetPosInfo().movedir());

				stat = info->mutable_statinfo();
				stat->set_attack(player->GetStat().attack());
				stat->set_hp(player->GetStat().hp());
				stat->set_level(player->GetStat().level());
				stat->set_maxhp(player->GetStat().maxhp());
				stat->set_speed(player->GetStat().speed());
				stat->set_totalexp(player->GetStat().totalexp());

				cout << p->name() << endl;

				auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterPacket);
				player->_session->Send(sendBuffer);

				Protocol::S_SPAWN spawnPacket;
				for(auto p : _players)
				{
					if (player != p.second)
						spawnPacket.mutable_objects()->Add(p.second->GetObjectInfo());
				}

				for (auto m : _monsters)
					spawnPacket.mutable_objects()->Add(m.second->GetObjectInfo());

				for(auto p : _projectiles)
					spawnPacket.mutable_objects()->Add(p.second->GetObjectInfo());

				sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);
				player->_session->Send(sendBuffer);
			}
		}
		else if (type == Protocol::GameObjectType::MONSTER)
		{
			MonsterPtr monster = dynamic_cast<MonsterPtr>(gameObject);
			_monsters[gameObject->GetId()] = monster;
			monster->Room = this;

			_map->ApplyMove(monster, Vector2Int(monster->GetCellPos().x, monster->GetCellPos().y));
		}
		else if (type == Protocol::GameObjectType::PROJECTILE)
		{
			ProjectilePtr projectile = dynamic_cast<Projectile*>(gameObject);
			_projectiles[gameObject->GetId()] = projectile;
			projectile->Room = this;
		}

		// 타인한테 정보 전송
		{
			Protocol::S_SPAWN spawnPacket;
			spawnPacket.mutable_objects()->Add(gameObject->GetObjectInfo());
			for(auto p : _players)
			{
				if (p.second->GetId() != gameObject->GetId())
				{
					auto sendBuffer = ClientPacketHandler::MakeSendBuffer(spawnPacket);
					p.second->_session->Send(sendBuffer);
				}
			}
		}
	}

	void GameRoom::LeaveGame(int32 id)
	{
		Protocol::GameObjectType type = ObjectManager::GetInst()->GetObjectTypeById(id);

		if (type == Protocol::GameObjectType::PLAYER)
		{
			PlayerPtr player = nullptr;
			player = _players[id];

			_map->ApplyLeave(player);
			player->Room = nullptr;

			if (_players.erase(id) == false)
				return;

			// 본인한테 정보 전송
			{
				Protocol::S_LEAVEGAME leavePacket;
				auto pkt = ClientPacketHandler::MakeSendBuffer(leavePacket);
				player->_session->Send(pkt);
			}
		}
		else if (type == Protocol::GameObjectType::MONSTER)
		{
			MonsterPtr monster = nullptr;

			if (_monsters.find(id) == _monsters.end())
				return;

			monster = _monsters[id];

			_map->ApplyLeave(monster);
			monster->Room = nullptr;

			_monsters.erase(id);
		}
		else if (type == Protocol::GameObjectType::PROJECTILE)
		{
			ProjectilePtr projectile = nullptr;

			if (_projectiles.find(id) == _projectiles.end())
				return;

			projectile = _projectiles[id];
			projectile->Room = nullptr;

			if (_projectiles.erase(id) == false)
				return;
		}

		// 타인한테 정보 전송
		{
			Protocol::S_DESPAWN despawnPacket;
			despawnPacket.mutable_objectids()->Add(id);
			for(auto p : _players)
			{
				if (p.second->GetId() != id)
				{
					auto pkt = ClientPacketHandler::MakeSendBuffer(despawnPacket);
					p.second->_session->Send(pkt);
				}
			}
		}
	}

	void GameRoom::HandleMove(PlayerPtr player, Protocol::C_MOVE movePacket)
	{
		if (player == nullptr)
			return;

		// TODO : 검증
		Protocol::PositionInfo movePosInfo = movePacket.posinfo();
		Protocol::ObjectInfo info = player->GetObjectInfo();

		// 다른 좌표로 이동할 경우, 갈 수 있는지 체크
		if (movePosInfo.posx() != info.posinfo().posx() || movePosInfo.posy() != info.posinfo().posy())
		{
			if (_map->CanGo(Vector2Int(movePosInfo.posx(), movePosInfo.posy())) == false)
				return;
		}

		info.mutable_posinfo()->set_state(movePosInfo.state());
		info.mutable_posinfo()->set_movedir(movePosInfo.movedir());
		_map->ApplyMove(player, Vector2Int(movePosInfo.posx(), movePosInfo.posy()));

		// 다른 플레이어한테도 알려준다
		Protocol::S_MOVE resMovePacket;
		resMovePacket.set_objectid(player->GetObjectInfo().objectid());
		resMovePacket.mutable_posinfo()->set_posx(movePacket.posinfo().posx());
		resMovePacket.mutable_posinfo()->set_posy(movePacket.posinfo().posy());
		resMovePacket.mutable_posinfo()->set_movedir(movePacket.posinfo().movedir());
		resMovePacket.mutable_posinfo()->set_state(movePacket.posinfo().state());

		auto pkt = ClientPacketHandler::MakeSendBuffer(resMovePacket);
		Broadcast(pkt);
	}

	void GameRoom::HandleSkill(PlayerPtr player, Protocol::C_SKILL skillPacket)
	{
		if (player == nullptr)
			return;

		Protocol::ObjectInfo info = player->GetObjectInfo();
		if (info.posinfo().state() != Protocol::CreatureState::IDLE)
			return;

		// TODO : 스킬 사용 가능 여부 체크
		info.mutable_posinfo()->set_state(Protocol::CreatureState::SKILL);
		Protocol::S_SKILL skill;
		skill.set_objectid(info.objectid());
		skill.mutable_info()->set_skillid(skillPacket.info().skillid());
		auto pkt = ClientPacketHandler::MakeSendBuffer(skill);
		Broadcast(pkt);

		Skill* skillData = nullptr;
		auto skillMap = DataManager::GetInst()->GetSkillMap();
		if (skillMap.find(skillPacket.info().skillid()) == skillMap.end())
			return;
		else
			skillData = skillMap[skillPacket.info().skillid()];

		switch (skillData->skillType)
		{
		case Protocol::SkillType::SKILL_AUTO :
		{
			Vector2Int skillPos = player->GetFrontCellPos(info.posinfo().movedir());
			GameObjectPtr target = _map->Find(skillPos);
			if (target != nullptr)
			{
				cout << "Hit GameObject !" << endl;
			}
		}
		break;
		case Protocol::SkillType::SKILL_PROJECTILE:
		{
			ArrowPtr arrow = ObjectManager::GetInst()->Add<Arrow>();
			if (arrow == nullptr)
				return;
		
			arrow->SetOwner(player);
			arrow->SetSkill(skillData);

			Protocol::PositionInfo posInfo;
			posInfo.set_state(Protocol::CreatureState::MOVING);
			posInfo.set_posx(player->GetPosInfo().posx());
			posInfo.set_posy(player->GetPosInfo().posy());
			posInfo.set_movedir(player->GetPosInfo().movedir());
			arrow->SetPosInfo(posInfo);

			arrow->SetSpeed(skillData->projectile.speed);

			// Job에 넣을 것
			DoAsync(&GameRoom::EnterGame, dynamic_cast<GameObjectPtr>(arrow));
		}
		break;
		}
	}

	void GameRoom::Broadcast(SendBufferRef packet)
	{
		for(auto p : _players)
		{
			p.second->_session->Send(packet);
		}
	}

	PlayerPtr GameRoom::FindPlayer(function<bool(GameObjectPtr)>& func)
	{
		for (auto player : _players)
		{
			PlayerPtr p = player.second;
			if (func(p))
			{
				return p;
			}
		}
		return nullptr;
	}

	MapData* GameRoom::GetMapData() const
	{
		return _map;
	}

}