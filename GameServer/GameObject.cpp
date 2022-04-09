#include "pch.h"
#include "GameObject.h"

#include "ClientPacketHandler.h"
#include "GameSession.h"

#include "GameRoom.h"
#include "Protocol.pb.h"

namespace FrokEngine
{
	GameObject::GameObject() : 
		_cellPos(Vector2Int(0,0))
	{
		auto posinfo = _objInfo.mutable_posinfo();
		posinfo = &_posInfo;
		auto statInfo = _objInfo.mutable_statinfo();
		statInfo = &_statInfo;
	}

	GameObject::~GameObject()
	{
	}

	shared_ptr<GameRoom> GameObject::GetGameRoom() const
	{
		return _room;
	}

	void GameObject::SetGameRoom(shared_ptr<GameRoom> room)
	{
		_room = room;
	}

	void GameObject::OnDamaged(GameObjectRef attacker, int damage)
	{
		if (_room == nullptr)
			return;

		_statInfo.set_hp(max(_statInfo.hp() - damage, 0));

		Protocol::S_CHANGEHP changePacket;
		changePacket.set_objectid(_objInfo.objectid());
		changePacket.set_hp(_statInfo.hp());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(changePacket);
		_room->Broadcast(sendBuffer);

		if (_statInfo.hp() <= 0)
		{
			OnDead(attacker);
		}
	}

	void GameObject::OnDead(GameObjectRef attacker)
	{
		if (_room == nullptr)
			return;

		Protocol::S_DIE diePacket;
		diePacket.set_objectid(_objInfo.objectid());
		diePacket.set_attackerid(attacker->GetObjectInfo().objectid());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(diePacket);
		_room->Broadcast(sendBuffer);

		// ¸¸µéÀÚ!
		shared_ptr<GameRoom> room = _room;
		room->LeaveGame(_objInfo.objectid());

		_statInfo.set_hp(_statInfo.maxhp());
		_posInfo.set_state(Protocol::CreatureState::IDLE);
		_posInfo.set_movedir(Protocol::MoveDir::DOWN);
		_posInfo.set_posx(0);
		_posInfo.set_posy(0);

		room->EnterGame((GameObjectRef)this);
	}

	void GameObject::Update()
	{

	}
}