#pragma once
#include "Projectile.h"

namespace FrokEngine
{
	class Arrow :
		public Projectile
	{
	public:
		void Update() override;

		void SetOwner(GameObjectRef gameObject) { _owner = gameObject; }

	private:
		long _nextMoveTick = 0;
		GameObjectRef _owner;
	};

}
