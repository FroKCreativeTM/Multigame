#pragma once
#include "Projectile.h"

namespace FrokEngine
{
	class Arrow :
		public Projectile
	{
	public:
		void Update() override;
		void SetOwner(GameObjectPtr gameObject) { _owner = gameObject; }

	private:
		long _nextMoveTick = 0;
		GameObjectPtr _owner;
	};

}
