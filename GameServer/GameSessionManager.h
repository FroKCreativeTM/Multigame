#pragma once

namespace FrokEngine
{
	class GameSession;

	using GameSessionRef = shared_ptr<GameSession>;

	class GameSessionManager
	{
	public:
		void Add(GameSessionRef session);
		void Remove(GameSessionRef session);
		void Broadcast(SendBufferRef sendBuffer);

	private:
		USE_LOCK;
		Set<GameSessionRef> _sessions;
		int32 _sessionId = 0;
	};

	extern GameSessionManager GSessionManager;

}