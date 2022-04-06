#include "pch.h"
#include "ClientPacketHandler.h"
#include "GameSession.h"

#include "DBConnector.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

// ���� ������ �۾���

bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_C_ENTERGAME(PacketSessionRef& session, Protocol::C_ENTERGAME& pkt)
{
	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);

	// TODO : Validation üũ

	// �����
	//Protocol::S_ENTERGAME loginPkt;
	//loginPkt.set_success(true);

	//// DB���� �÷��� ������ �ܾ�´�
	//// GameSession�� �÷��� ������ ���� (�޸�)

	//// ID �߱� (DB ���̵� �ƴϰ�, �ΰ��� ���̵�)
	//static Atomic<uint64> idGenerator = 1;

	//auto stmt = GDBConnector->GetStatement();
	//string query = "SELECT * FROM playerleveltable p WHERE p.level = 1";

	//auto ret = stmt->executeQuery(query.c_str());

	//Protocol::LevelInfo level;

	//while (ret->next())
	//{
	//	level.set_level(ret->getInt("Level"));
	//	level.set_maxhp(ret->getInt("MaxHP"));
	//	level.set_attack(ret->getInt("Attack"));
	//	level.set_dropexp(ret->getInt("DropExp"));
	//	level.set_nextexp(ret->getInt("NextExp"));
	//}

	//auto player = loginPkt.mutable_player();
	//player->set_level(level.level());
	//player->set_maxhp(level.maxhp());
	//player->set_attack(level.attack());
	//player->set_dropexp(level.dropexp());
	//player->set_nextexp(level.nextexp());

	//PlayerRef playerRef = MakeShared<Player>();
	//playerRef->playerId = idGenerator++;
	//playerRef->levelinfo = level;
	//playerRef->ownerSession = gameSession;

	//gameSession->_players.push_back(playerRef);

	//auto sendBuffer = ClientPacketHandler::MakeSendBuffer(loginPkt);
	//session->Send(sendBuffer);

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
	return true;
}

bool Handle_C_SKILL(PacketSessionRef& session, Protocol::C_SKILL& pkt)
{
	return false;
}

bool Handle_C_CHANGEHP(PacketSessionRef& session, Protocol::C_CHANGEHP& pkt)
{
	return false;
}

bool Handle_C_DIE(PacketSessionRef& session, Protocol::C_DIE& pkt)
{
	return false;
}
//
//bool Handle_C_ENTER_GAME(PacketSessionRef& session, Protocol::C_ENTER_GAME& pkt)
//{
//	GameSessionRef gameSession = static_pointer_cast<GameSession>(session);
//
//	uint64 index = pkt.playerindex();
//	// TODO : Validation
//
//	gameSession->_currentPlayer = gameSession->_players[index]; // READ_ONLY?
//	gameSession->_room = GRoom;
//
//	GRoom->DoAsync(&Room::Enter, gameSession->_currentPlayer);
//
//	Protocol::S_ENTERGAME enterGamePkt;
//	enterGamePkt.set_success(true);
//	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(enterGamePkt);
//	gameSession->_currentPlayer->ownerSession->Send(sendBuffer);
//
//	return true;
//}
//
//bool Handle_C_CHAT(PacketSessionRef& session, Protocol::C_CHAT& pkt)
//{
//	std::cout << pkt.msg() << endl;
//
//	Protocol::S_CHAT chatPkt;
//	chatPkt.set_msg(pkt.msg());
//	auto sendBuffer = ClientPacketHandler::MakeSendBuffer(chatPkt);
//
//	GRoom->DoAsync(&Room::Broadcast, sendBuffer);
//
//	return true;
//}
