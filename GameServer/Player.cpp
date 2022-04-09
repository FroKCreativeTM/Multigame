#include "pch.h"
#include "Player.h"

namespace FrokEngine
{
	void Player::OnDamaged(GameObjectPtr attacker, int damage)
	{
		GameObject::OnDamaged(attacker, damage);
	}

	void Player::OnDead(GameObjectPtr attacker)
	{
		GameObject::OnDead(attacker);
	}
}

