#pragma once

#include "GameObject.h"

namespace FrokEngine
{
	class Player :
		public GameObject
	{
	public :
		PacketSession* _session = nullptr;

		Player() 
		{
			_objectType = Protocol::GameObjectType::PLAYER;
		}

		void OnDamaged(GameObjectPtr attacker, int damage) override;
		void OnDead(GameObjectPtr attacker) override;
	};
}

