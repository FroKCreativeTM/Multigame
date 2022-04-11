#include "pch.h"
#include "GameSession.h"
#include "GameSessionManager.h"
#include "ClientPacketHandler.h"
#include "GameRoom.h"
#include "ObjectManager.h"
#include "Player.h"
#include "DataManager.h"
#include "RoomManager.h"

namespace FrokEngine
{
	void GameSession::OnConnected()
	{
		cout << "OnConnected : {endPoint}" << endl;

		_currentPlayer = ObjectManager::GetInst()->Add<Player>();
		Protocol::ObjectInfo objInfo;
		objInfo.set_name(u8"Player_" + to_string(_currentPlayer->GetObjectInfo().objectid()));
		auto curObjInfo = _currentPlayer->GetObjectInfo();
		auto posInfo = curObjInfo.mutable_posinfo();
		posInfo->set_state(Protocol::CreatureState::IDLE);
		posInfo->set_movedir(Protocol::MoveDir::DOWN);
		posInfo->set_posx(0);
		posInfo->set_posx(0);

		Protocol::StatInfo stat;
		auto statdata = DataManager::GetInst()->GetStatMap();
		if (statdata.find(1) != statdata.end())
			stat = *statdata[1];
		_currentPlayer->SetStat(stat);

		_currentPlayer->_session = dynamic_cast<PacketSession*>(this);

		if (_currentPlayer)
		{
			if (auto room = _room.lock())
				room->DoAsync(&GameRoom::EnterGame, dynamic_cast<GameObjectPtr>(_currentPlayer));
		}

		GSessionManager.Add(static_pointer_cast<GameSession>(shared_from_this()));
	}

	void GameSession::OnDisconnected()
	{
		GSessionManager.Remove(static_pointer_cast<GameSession>(shared_from_this()));

		if (_currentPlayer)
		{
			if (auto room = _room.lock())
				room ->DoAsync(&GameRoom::LeaveGame, _currentPlayer->GetId());
		}

		_currentPlayer = nullptr;
		_players.clear();
	}

	void GameSession::OnRecvPacket(BYTE* buffer, int32 len)
	{
		PacketSessionRef session = GetPacketSessionRef();
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);

		// TODO : packetId 대역 체크
		ClientPacketHandler::HandlePacket(session, buffer, len);
	}

	void GameSession::OnSend(int32 len)
	{
	}
}