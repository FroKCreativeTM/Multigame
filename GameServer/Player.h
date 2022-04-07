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

		void OnDamaged(PacketSessionRef& session, GameObject* attacker, int damage) override
		{
			GameObject::OnDamaged(_session, attacker, damage);
		}
		void OnDead(PacketSessionRef& session, GameObject* attacker) override
		{
			GameObject::OnDead(_session, attacker);
		}
	};
}

