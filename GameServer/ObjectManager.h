#pragma once

#include "CorePch.h"

namespace FrokEngine
{
	class ObjectManager
	{
		static ObjectManager* GetInst()
		{
			if (!_inst)
				_inst = new ObjectManager();
			return _inst;
		}

		template <typename T>
		T Add()
		{
			T gameObject = new T();

			// 락 걸고
			WRITE_LOCK;
			{
				gameObject.set_id(GenerateId(gameObject.ObjectType));

				if (gameObject.ObjectType == GameObjectType.Player)
				{
					_players.Add(gameObject.Id, gameObject as Player);
				}
			}

			return gameObject;
		}

	private :
		USE_LOCK;	// 락 쓸꺼임
		static ObjectManager* _inst;
		int _counter = 0;
		map<int, class Player> _players;
	};
}

