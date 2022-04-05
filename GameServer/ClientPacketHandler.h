#pragma once
#include "Protocol.pb.h"

using PacketHandlerFunc = std::function<bool(PacketSessionRef&, BYTE*, int32)>;
extern PacketHandlerFunc GPacketHandler[UINT16_MAX];

enum : uint16
{
	PKT_S_ENTERGAME = 1000,
	PKT_C_ENTERGAME = 1001,
	PKT_S_LEAVEGAME = 1002,
	PKT_C_LEAVEGAME = 1003,
	PKT_S_SPAWN = 1004,
	PKT_C_SPAWN = 1005,
	PKT_S_DESPAWN = 1006,
	PKT_C_DESPAWN = 1007,
	PKT_S_MOVE = 1008,
	PKT_C_MOVE = 1009,
	PKT_S_SKILL = 1010,
	PKT_C_SKILL = 1011,
	PKT_S_CHANGEHP = 1012,
	PKT_C_CHANGEHP = 1013,
	PKT_S_DIE = 1014,
	PKT_C_DIE = 1015,
};

// Custom Handlers
bool Handle_INVALID(PacketSessionRef& session, BYTE* buffer, int32 len);
bool Handle_C_ENTERGAME(PacketSessionRef& session, Protocol::C_ENTERGAME& pkt);
bool Handle_C_LEAVEGAME(PacketSessionRef& session, Protocol::C_LEAVEGAME& pkt);
bool Handle_C_SPAWN(PacketSessionRef& session, Protocol::C_SPAWN& pkt);
bool Handle_C_DESPAWN(PacketSessionRef& session, Protocol::C_DESPAWN& pkt);
bool Handle_C_MOVE(PacketSessionRef& session, Protocol::C_MOVE& pkt);
bool Handle_C_SKILL(PacketSessionRef& session, Protocol::C_SKILL& pkt);
bool Handle_C_CHANGEHP(PacketSessionRef& session, Protocol::C_CHANGEHP& pkt);
bool Handle_C_DIE(PacketSessionRef& session, Protocol::C_DIE& pkt);

class ClientPacketHandler
{
public:
	static void Init()
	{
		for (int32 i = 0; i < UINT16_MAX; i++)
			GPacketHandler[i] = Handle_INVALID;
		GPacketHandler[PKT_C_ENTERGAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_ENTERGAME>(Handle_C_ENTERGAME, session, buffer, len); };
		GPacketHandler[PKT_C_LEAVEGAME] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_LEAVEGAME>(Handle_C_LEAVEGAME, session, buffer, len); };
		GPacketHandler[PKT_C_SPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SPAWN>(Handle_C_SPAWN, session, buffer, len); };
		GPacketHandler[PKT_C_DESPAWN] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_DESPAWN>(Handle_C_DESPAWN, session, buffer, len); };
		GPacketHandler[PKT_C_MOVE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_MOVE>(Handle_C_MOVE, session, buffer, len); };
		GPacketHandler[PKT_C_SKILL] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_SKILL>(Handle_C_SKILL, session, buffer, len); };
		GPacketHandler[PKT_C_CHANGEHP] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_CHANGEHP>(Handle_C_CHANGEHP, session, buffer, len); };
		GPacketHandler[PKT_C_DIE] = [](PacketSessionRef& session, BYTE* buffer, int32 len) { return HandlePacket<Protocol::C_DIE>(Handle_C_DIE, session, buffer, len); };
	}

	static bool HandlePacket(PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
		return GPacketHandler[header->id](session, buffer, len);
	}
	static SendBufferRef MakeSendBuffer(Protocol::S_ENTERGAME& pkt) { return MakeSendBuffer(pkt, PKT_S_ENTERGAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_LEAVEGAME& pkt) { return MakeSendBuffer(pkt, PKT_S_LEAVEGAME); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_SPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_DESPAWN& pkt) { return MakeSendBuffer(pkt, PKT_S_DESPAWN); }
	static SendBufferRef MakeSendBuffer(Protocol::S_MOVE& pkt) { return MakeSendBuffer(pkt, PKT_S_MOVE); }
	static SendBufferRef MakeSendBuffer(Protocol::S_SKILL& pkt) { return MakeSendBuffer(pkt, PKT_S_SKILL); }
	static SendBufferRef MakeSendBuffer(Protocol::S_CHANGEHP& pkt) { return MakeSendBuffer(pkt, PKT_S_CHANGEHP); }
	static SendBufferRef MakeSendBuffer(Protocol::S_DIE& pkt) { return MakeSendBuffer(pkt, PKT_S_DIE); }

private:
	template<typename PacketType, typename ProcessFunc>
	static bool HandlePacket(ProcessFunc func, PacketSessionRef& session, BYTE* buffer, int32 len)
	{
		PacketType pkt;
		if (pkt.ParseFromArray(buffer + sizeof(PacketHeader), len - sizeof(PacketHeader)) == false)
			return false;

		return func(session, pkt);
	}

	template<typename T>
	static SendBufferRef MakeSendBuffer(T& pkt, uint16 pktId)
	{
		const uint16 dataSize = static_cast<uint16>(pkt.ByteSizeLong());
		const uint16 packetSize = dataSize + sizeof(PacketHeader);

		SendBufferRef sendBuffer = GSendBufferManager->Open(packetSize);
		PacketHeader* header = reinterpret_cast<PacketHeader*>(sendBuffer->Buffer());
		header->size = packetSize;
		header->id = pktId;
		ASSERT_CRASH(pkt.SerializeToArray(&header[1], dataSize));
		sendBuffer->Close(packetSize);

		return sendBuffer;
	}
};