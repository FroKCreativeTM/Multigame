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
	void GameRoom::Init(int mapId)
	{
		_map->LoadMap(mapId);

		// TEMP
		MonsterRef monster = ObjectManager::GetInst()->Add<MonsterRef>();
		monster->SetCellPos(5, 5);
		EnterGame(static_pointer_cast<GameObject>(monster));
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

	void GameRoom::EnterGame(GameObjectRef gameObject)
	{
		if (gameObject == nullptr)
			return;

		Protocol::GameObjectType type = ObjectManager::GetInst()->GetObjectTypeById(gameObject->GetId());

		if (type == Protocol::GameObjectType::PLAYER)
		{
			PlayerRef player = static_pointer_cast<Player>(gameObject);
			_players.insert(make_pair(gameObject->GetId(), player));

			player->SetGameRoom(GRoom);

			_map->ApplyMove(player, Vector2Int(player->GetCellPos().x, player->GetCellPos().y));

			// 본인한테 정보 전송
			{
				Protocol::S_ENTERGAME enterPacket;
				auto info = enterPacket.mutable_player();
				info->set_objectid(player->GetObjectInfo().objectid());
				info->set_name(player->GetObjectInfo().name());
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
			MonsterRef monster = static_pointer_cast<Monster>(gameObject);
			_monsters.insert(make_pair(gameObject->GetId(), monster));
			monster->SetGameRoom(GRoom);

			_map->ApplyMove(monster, Vector2Int(monster->GetCellPos().x, monster->GetCellPos().y));
		}
		else if (type == Protocol::GameObjectType::PROJECTILE)
		{
			ProjectileRef projectile = static_pointer_cast<Projectile>(gameObject);
			_projectiles.insert(make_pair(gameObject->GetId(), projectile));
			projectile->SetGameRoom(GRoom);
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
			PlayerRef player = nullptr;
			if (_players.erase(id) == false)
				return;

			_map->ApplyLeave(player);
			player->SetGameRoom(nullptr);

			// 본인한테 정보 전송
			{
				Protocol::S_LEAVEGAME leavePacket;
				auto pkt = ClientPacketHandler::MakeSendBuffer(leavePacket);
				player->_session->Send(pkt);
			}
		}
		else if (type == Protocol::GameObjectType::MONSTER)
		{
			MonsterRef monster = nullptr;
			if (_monsters.erase(id) == false)
				return;

			_map->ApplyLeave(monster);
			monster->SetGameRoom(nullptr);
		}
		else if (type == Protocol::GameObjectType::PROJECTILE)
		{
			Projectile* projectile = nullptr;
			if (_projectiles.erase(id) == false)
				return;

			projectile->SetGameRoom(nullptr);
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

	void GameRoom::HandleMove(PlayerRef player, Protocol::C_MOVE movePacket)
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

	void GameRoom::HandleSkill(PlayerRef player, Protocol::C_SKILL skillPacket)
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

		shared_ptr<Data::Skill> skillData = nullptr;
		if (DataManager::GetSkillMap().find(skillPacket.info().skillid()) == DataManager::GetSkillMap().end())
			return;
		else
			skillData = DataManager::GetSkillMap()[skillPacket.info().skillid()];

		switch (skillData->skillType)
		{
		case Protocol::SkillType::SKILL_AUTO :
		{
			Vector2Int skillPos = player->GetFrontCellPos(info.posinfo().movedir());
			GameObjectRef target = _map->Find(skillPos);
			if (target != nullptr)
			{
				cout << "Hit GameObject !" << endl;
			}
		}
		break;
		case Protocol::SkillType::SKILL_PROJECTILE:
		{
			ArrowRef arrow = ObjectManager::GetInst()->Add<ArrowRef>();
			if (arrow == nullptr)
				return;
		
			arrow->SetOwner(player);
			arrow->_data = skillData;

			Protocol::PositionInfo posInfo;
			posInfo.set_state(Protocol::CreatureState::MOVING);
			posInfo.set_posx(player->GetPosInfo().posx());
			posInfo.set_posy(player->GetPosInfo().posy());
			posInfo.set_movedir(player->GetPosInfo().movedir());
			arrow->SetPosInfo(posInfo);

			arrow->SetSpeed(skillData->projectile->speed);

			// Job에 넣을 것
			DoAsync(EnterGame, arrow);
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

	PlayerRef GameRoom::FindPlayer(function<bool(GameObjectRef)>& func)
	{
		for (auto player : _players)
		{
			PlayerRef p = player.second;
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