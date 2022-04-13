#pragma once
#include "GameObject.h"
#include "Contents.h"

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
			_data = new Skill();
			_objectType = Protocol::GameObjectType::PROJECTILE;
		}

		~Projectile()
		{
			if (_data)
			{
				delete _data;
			}
			_data = nullptr;
		}

		virtual void Update()
		{
		}

	private : 
		Skill* _data;
	};
}

