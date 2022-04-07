#include "pch.h"
#include "GameRoom.h"

#include "ClientPacketHandler.h"

#include "MapData.h"

#include "Player.h"
#include "Monster.h"
#include "ObjectManager.h"
#include "Projectile.h"

namespace FrokEngine
{
	void GameRoom::Init(PacketSessionRef& session, int mapId)
	{
		_map->LoadMap(mapId);

		// TEMP
		Monster monster = ObjectManager::GetInst()->Add<Monster>();
		monster.SetCellPos(5, 5);
		EnterGame(session, &monster);
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

		// Flush(); // job
	}

	void GameRoom::EnterGame(PacketSessionRef& session, GameObject* gameObject)
	{
		if (gameObject == nullptr)
			return;

		Protocol::GameObjectType type = ObjectManager::GetInst()->GetObjectTypeById(gameObject->GetId());

		if (type == Protocol::GameObjectType::PLAYER)
		{
			Player* player = (Player*)gameObject;
			_players.insert(make_pair(gameObject->GetId(), player));
			player->SetGameRoom(this);

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
			Monster* monster = (Monster*)gameObject;
			_monsters.insert(make_pair(gameObject->GetId(), monster));
			monster->SetGameRoom(this);

			_map->ApplyMove(monster, Vector2Int(monster->GetCellPos().x, monster->GetCellPos().y));
		}
		else if (type == Protocol::GameObjectType::PROJECTILE)
		{
			Projectile* projectile = (Projectile*)gameObject;
			_projectiles.insert(make_pair(gameObject->GetId(), projectile));
			projectile->SetGameRoom(this);
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

	void GameRoom::LeaveGame(PacketSessionRef& session, int32 id)
	{

		Protocol::GameObjectType type = ObjectManager::GetInst()->GetObjectTypeById(id);

		if (type == Protocol::GameObjectType::PLAYER)
		{
			Player* player = nullptr;
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
			Monster* monster = nullptr;
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

	void GameRoom::HandleMove(PacketSessionRef& session, Player* player, Protocol::C_MOVE movePacket)
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

	void GameRoom::HandleSkill(PacketSessionRef& session, Player* player, Protocol::C_SKILL skillPacket)
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

		// Data.Skill skillData = null;
		// if (DataManager.SkillDict.TryGetValue(skillPacket.Info.SkillId, out skillData) == false)
		// 	return;

		// switch (skillData.skillType)
		// {
		// case SkillType.SkillAuto:
		// {
		// 	Vector2Int skillPos = player.GetFrontCellPos(info.PosInfo.MoveDir);
		// 	GameObject target = Map.Find(skillPos);
		// 	if (target != null)
		// 	{
		// 		Console.WriteLine("Hit GameObject !");
		// 	}
		// }
		// break;
		// case SkillType.SkillProjectile:
		// {
		// 	Arrow arrow = ObjectManager.Instance.Add<Arrow>();
		// 	if (arrow == null)
		// 		return;
		// 
		// 	arrow.Owner = player;
		// 	arrow.Data = skillData;
		// 	arrow.PosInfo.State = CreatureState.Moving;
		// 	arrow.PosInfo.MoveDir = player.PosInfo.MoveDir;
		// 	arrow.PosInfo.PosX = player.PosInfo.PosX;
		// 	arrow.PosInfo.PosY = player.PosInfo.PosY;
		// 	arrow.Speed = skillData.projectile.speed;
		// 	Push(EnterGame, arrow);
		// }
		// break;
		// }
	}

	void GameRoom::Broadcast(SendBufferRef packet)
	{
	}

	MapData* GameRoom::GetMapData() const
	{
		return nullptr;
	}

}