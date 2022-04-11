#pragma once

#include "CorePch.h"

#include "Enum.pb.h"
#include "Struct.pb.h"
#include "Protocol.pb.h"

namespace FrokEngine
{
	class ObjectManager
	{
	public : 
		static ObjectManager* GetInst()
		{
			if (!_inst)
				_inst = new ObjectManager();
			return _inst;
		}

		// 이거 고쳐야됨
		template <typename T>
		T* Add()
		{
			GameObjectPtr gameObject = new T;

			// 락 걸고
			WRITE_LOCK;
			gameObject->SetId(GenerateId(gameObject->GetGameObjectType()));

			if (gameObject->GetGameObjectType() == Protocol::GameObjectType::PLAYER)
			{
				_players[gameObject->GetId()] = dynamic_cast<PlayerPtr>(gameObject);
			}

			return dynamic_cast<T*>(gameObject);
		}

		int GenerateId(Protocol::GameObjectType type)
		{
			READ_LOCK;
			return ((int)type << 24) | (_counter++);
		}

		static Protocol::GameObjectType GetObjectTypeById(int id)
		{
			int type = (id >> 24) & 0x7F;
			return (Protocol::GameObjectType)type;
		}

		bool Remove(int objectId)
		{
			Protocol::GameObjectType objectType = GetObjectTypeById(objectId);

			WRITE_LOCK;
			if (objectType == Protocol::GameObjectType::PLAYER)
				return _players.erase(objectId);

			return false;
		}

		PlayerPtr Find(int objectId)
		{
			Protocol::GameObjectType objectType = GetObjectTypeById(objectId);

			READ_LOCK;
			if (objectType == Protocol::GameObjectType::PLAYER)
			{
				if (_players.find(objectId) != _players.end())
					return _players.find(objectId)->second;
			}

			return nullptr;
		}

	private :
		USE_LOCK;	// 락 쓸꺼임
		static ObjectManager* _inst;
		int _counter = 0;
		map<int, PlayerPtr> _players;
	};
}

