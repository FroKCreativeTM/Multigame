#include "pch.h"
#include "ClientPacketHandler.h"
#include "GameSession.h"

#include "DBConnector.h"

#include "Player.h"
#include "GameRoom.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// Á÷Á¢ ÄÁÅÙÃ÷ ÀÛ¾÷ÀÚ

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_ENTERGAME(PacketSessionRef& session, Protocol::C_ENTERGAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);


	return true;
}

bool Handle_C_LEAVEGAME(PacketSessionRef& session, Protocol::C_LEAVEGAME& pkt)
{
	return false;
}

bool Handle_C_SPAWN(PacketSessionRef& session, Protocol::C_SPAWN& pkt)
{
	return false;
}

bool Handle_C_DESPAWN(PacketSessionRef& session, Protocol::C_DESPAWN& pkt)
{
	return false;
}

bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt)
{
	Protocol::C_MOVE movePacket = pkt;
	//Console.WriteLine($"C_Move ({movePacket.PosInfo.PosX}, {movePacket.PosInfo.PosY})");
	GameSession* game = dynamic_cast<GameSession*>(session.get());

	Player* player = game->GetPlayer();
	if (player == nullptr)
		return false;
	
	GameRoom* room = player->Room;
	if (room == nullptr)
		return false;
	
	room->DoAsync(&GameRoom::HandleMove, player, movePacket);

	return true;
}

bool Handle_C_SKILL(PacketSessionRef& session, Protocol::C_SKILL& pkt)
{
	Protocol::C_SKILL skillPacket = pkt;
	GameSession* game = dynamic_cast<GameSession*>(session.get());
	
	Player* player = game->GetPlayer();
	if (player == nullptr)
		return false;
	
	GameRoom* room = player->Room;
	if (room == nullptr)
		return false;

	room->DoAsync(&GameRoom::HandleSkill, player, skillPacket);

	return true;
}

bool Handle_C_CHANGEHP(PacketSessionRef& session, Protocol::C_CHANGEHP& pkt)
{
	return true;
}

bool Handle_C_DIE(PacketSessionRef& session, Protocol::C_DIE& pkt)
{
	return true;
}