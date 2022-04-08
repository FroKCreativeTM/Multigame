#pragma once
#include "GameObject.h"

namespace FrokEngine
{
	class Monster :
		public GameObject
	{
	public:
		Monster();

		// FSM (Finite State Machine)
		void Update() override;

	protected:
		virtual void UpdateIdle();
		virtual void UpdateMoving();
		virtual void UpdateSkill();
		virtual void UpdateDead();

	private:
		void BroadcastMove();

	protected:
		PlayerRef	_target;
		int				_searchCellDist = 10;
		int				_chaseCellDist = 20;
		uint64			_nextSearchTick = 0;
		int				_skillRange = 1;
		long			_nextMoveTick = 0;
		long			_coolTick = 0;
	};


}