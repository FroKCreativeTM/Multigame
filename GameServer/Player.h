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

		void OnDamaged(GameObjectRef attacker, int damage) override;
		void OnDead(GameObjectRef attacker) override;
	};
}

