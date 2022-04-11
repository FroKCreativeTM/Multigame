#include "pch.h"
#include "Projectile.h"
#include "DataManager.h"

namespace FrokEngine
{
	Skill* Projectile::GetData()
	{
		return _data;
	}

	void Projectile::SetSkill(const Skill* skill)
	{
		_data->id = skill->id;
		_data->name = skill->name;
		_data->damage = skill->damage;
		_data->cooldown = skill->cooldown;
		_data->skillType = skill->skillType;
		_data->projectile = skill->projectile;
	}
}
