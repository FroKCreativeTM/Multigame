#pragma once
#include "GameObject.h"
#include "DataManager.h"

namespace FrokEngine
{
	class Projectile :
		public GameObject
	{
	public:
		shared_ptr<Data::Skill> _data;

		Projectile()
		{
			_objectType = Protocol::GameObjectType::PROJECTILE;
		}

		virtual void Update()
		{
		}
	};
}

