#include "pch.h"
#include "ServerPacketHandler.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// 직접 컨텐츠 작업자

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_ENTERGAME(PacketSessionRef& session, Protocol::S_ENTERGAME& pkt)
{
	if (pkt.success() == false)
		return true;

	if (pkt.players().size() == 0)
	{
		// 캐릭터 생성창
	}

	// 입장 UI 버튼 눌러서 게임 입장
	Protocol::S_ENTERGAME enterGamePkt;
	enterGamePkt.set_playerindex(0); // 첫번째 캐릭터로 입장
	auto sendBuffer = ServerPacketHandler::MakeSendBuffer(enterGamePkt);
	session->Send(sendBuffer);

	return true;
}

bool Handle_S_LEAVEGAME(PacketSessionRef& session, Protocol::S_LEAVEGAME& pkt)
{
	return false;
}

bool Handle_S_SPAWN(PacketSessionRef& session, Protocol::S_SPAWN& pkt)
{
	return false;
}

bool Handle_S_DESPAWN(PacketSessionRef& session, Protocol::S_DESPAWN& pkt)
{
	return false;
}

bool Handle_S_MOVE(PacketSessionRef& session, Protocol::S_MOVE& pkt)
{
	std::cout << "pos x : " << pkt.posinfo().posx() << 
		"\ty : " << pkt.posinfo().posx() << 
		"\tstate : " << pkt.posinfo().state() << endl;
	return true;
}

bool Handle_S_SKILL(PacketSessionRef& session, Protocol::S_SKILL& pkt)
{
	return false;
}

bool Handle_S_CHANGEHP(PacketSessionRef& session, Protocol::S_CHANGEHP& pkt)
{
	return false;
}

bool Handle_S_DIE(PacketSessionRef& session, Protocol::S_DIE& pkt)
{
	return false;
}
