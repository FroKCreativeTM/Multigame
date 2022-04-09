#pragma once

#include "GameObject.h"

namespace FrokEngine
{
	class Player :
		public GameObject
	{
	public :
		PacketSessionRef& _session;

		Player(PacketSessionRef& session) : 
			_session(session)
		{
			_objectType = Protocol::GameObjectType::PLAYER;
		}

		void OnDamaged(GameObjectPtr attacker, int damage) override;
		void OnDead(GameObjectPtr attacker) override;
	};
}

