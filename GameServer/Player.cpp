#include "pch.h"
#include "Player.h"

namespace FrokEngine
{
	void Player::OnDamaged(GameObjectRef attacker, int damage)
	{
		GameObject::OnDamaged(attacker, damage);
	}

	void Player::OnDead(GameObjectRef attacker)
	{
		GameObject::OnDead(attacker);
	}
}

