#pragma once
#include "GameObject.h"

namespace FrokEngine
{
	class Projectile :
		public GameObject
	{
	public:
		Data::Skill Data;

		Projectile()
		{
			_objectType = Protocol::GameObjectType::PROJECTILE;
		}

		virtual void Update()
		{
		}
	};
}

