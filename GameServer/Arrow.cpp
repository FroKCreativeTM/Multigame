#include "pch.h"
#include "Arrow.h"

#include "ClientPacketHandler.h"

#include "GameRoom.h"

namespace FrokEngine
{
	void Arrow::Update()
	{
		if (_data == nullptr || _data->projectile == nullptr || _owner == nullptr || _room == nullptr)
			return;

		if (_nextMoveTick >= GetTickCount64())
			return;

		uint64 tick = (uint64)(1000 / _data->projectile->speed);
		_nextMoveTick = GetTickCount64() + tick;

		Vector2Int destPos = GetFrontCellPos();
		if (_room->GetMapData()->CanGo(destPos))
		{
			_cellPos = destPos;

			Protocol::S_MOVE movePacket;
			movePacket.set_objectid(GetId());
			auto pos = movePacket.mutable_posinfo();
			pos->set_posx(_posInfo.posx());
			pos->set_posy(_posInfo.posy());
			pos->set_state(_posInfo.state());
			pos->set_movedir(_posInfo.movedir());

			auto pkt = ClientPacketHandler::MakeSendBuffer(movePacket);
			_room->Broadcast(pkt);

			cout << "Move Arrow" << endl;
		}
		else
		{
			GameObjectRef target = _room->GetMapData()->Find(destPos);
			if (target != nullptr)
			{
				//Stat.Attack
				auto tmp = make_shared<GameObject>(this);
				target->OnDamaged(tmp, _data->damage + _owner->GetStat().attack());
			}

			// ¼Ò¸ê
			// GRoom->DoAsync(_room->LeaveGame, Id);
		}
	}

}