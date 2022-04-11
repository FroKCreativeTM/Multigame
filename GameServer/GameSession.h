#pragma once
#include "Session.h"

namespace FrokEngine
{
	class GameSession : public PacketSession
	{
	public:
		~GameSession()
		{
			cout << "~GameSession" << endl;
		}

		virtual void OnConnected() override;
		virtual void OnDisconnected() override;
		virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
		virtual void OnSend(int32 len) override;

		void Send(google::protobuf::Message& packet);

	public:
		Vector<PlayerPtr> _players;

		PlayerPtr _currentPlayer;
		weak_ptr<class GameRoom> GRoom;
	};
}