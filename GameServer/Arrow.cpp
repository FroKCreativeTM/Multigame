#include "pch.h"
#include "Arrow.h"
#include "ClientPacketHandler.h"
#include "GameRoom.h"

#include "DataManager.h"

namespace FrokEngine
{
	void Arrow::Update()
	{
		if (GetData() == nullptr || GetData()->projectile.name == "" || _owner == nullptr || _room == nullptr)
			return;

		if (_nextMoveTick >= GetTickCount64())
			return;

		uint64 tick = (uint64)(1000 / GetData()->projectile.speed);
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
			GameObjectPtr target = _room->GetMapData()->Find(destPos);
			if (target != nullptr)
			{
				//Stat.Attack
				target->OnDamaged((GameObjectPtr)this, GetData()->damage + _owner->GetStat().attack());
			}

			// �Ҹ�
			_room->DoAsync(&GameRoom::LeaveGame, GetId());
		}
	}

}