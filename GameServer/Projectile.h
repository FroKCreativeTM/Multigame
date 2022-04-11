#pragma once
#include "GameObject.h"

namespace FrokEngine
{
	class Skill;

	class Projectile :
		public GameObject
	{
	public:
		Skill* GetData();
		void SetSkill(const Skill* skill);

		Projectile()
		{
			_objectType = Protocol::GameObjectType::PROJECTILE;
		}

		virtual void Update()
		{
		}

	private : 
		Skill* _data;
	};
}

