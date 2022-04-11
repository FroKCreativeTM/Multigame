#include "pch.h"
#include "Arrow.h"
#include "ClientPacketHandler.h"
#include "GameRoom.h"

#include "DataManager.h"

namespace FrokEngine
{
	void Arrow::Update()
	{
		if (GetData() == nullptr || GetData()->projectile.name == "" || _owner == nullptr || Room == nullptr)
			return;

		if (_nextMoveTick >= GetTickCount64())
			return;

		uint64 tick = (uint64)(1000 / GetData()->projectile.speed);
		_nextMoveTick = GetTickCount64() + tick;

		Vector2Int destPos = GetFrontCellPos();
		if (Room->GetMapData()->CanGo(destPos))
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
			Room->Broadcast(pkt);

			cout << "Move Arrow" << endl;
		}
		else
		{
			GameObjectPtr target = Room->GetMapData()->Find(destPos);
			if (target != nullptr)
			{
				//Stat.Attack
				target->OnDamaged((GameObjectPtr)this, GetData()->damage + _owner->GetStat().attack());
			}

			// ¼Ò¸ê
			Room->DoAsync(&GameRoom::LeaveGame, GetId());
		}
	}

}