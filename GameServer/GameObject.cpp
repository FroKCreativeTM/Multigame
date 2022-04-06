#include "pch.h"
#include "GameObject.h"

#include "ClientPacketHandler.h"
#include "GameSession.h"

#include "GameRoom.h"
#include "Protocol.pb.h"

namespace FrokEngine
{
	GameObject::GameObject() 
	{
		auto posinfo = _objInfo.mutable_posinfo();
		posinfo = &_posInfo;
		auto statInfo = _objInfo.mutable_statinfo();
		statInfo = &_statInfo;
	}

	GameRoom* GameObject::GetGameRoom() const
	{
		return nullptr;
	}

	void GameObject::SetGameRoom(GameRoom* room)
	{

	}

	void GameObject::OnDamaged(PacketSessionRef& session, GameObject* attacker, int damage)
	{
		if (_room == nullptr)
			return;

		_statInfo.set_hp(max(_statInfo.hp() - damage, 0));

		Protocol::S_CHANGEHP changePacket;
		changePacket.set_objectid(_objInfo.objectid());
		changePacket.set_hp(_statInfo.hp());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(changePacket);
		session->Send(sendBuffer);

		if (_statInfo.hp() <= 0)
		{
			OnDead(session, attacker);
		}
	}

	void GameObject::OnDead(PacketSessionRef& session, GameObject* attacker)
	{
		if (_room == nullptr)
			return;

		Protocol::S_DIE diePacket;
		diePacket.set_objectid(_objInfo.objectid());
		diePacket.set_attackerid(attacker->GetObjectInfo().objectid());
		auto sendBuffer = ClientPacketHandler::MakeSendBuffer(diePacket);
		session->Send(sendBuffer);

		// ������!
		GameRoom* room = _room;
		room->LeaveGame(_objInfo.objectid());

		_statInfo.set_hp(_statInfo.maxhp());
		_posInfo.set_state(Protocol::CreatureState::IDLE);
		_posInfo.set_movedir(Protocol::MoveDir::DOWN);
		_posInfo.set_posx(0);
		_posInfo.set_posy(0);

		room->EnterGame(this);
	}

	void GameObject::Update()
	{

	}
}