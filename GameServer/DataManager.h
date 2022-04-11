#pragma once

#include "pch.h"
#include "Contents.h"
#include "ConfigManager.h"

namespace FrokEngine
{
	class DataManager
	{
	public : 
		static DataManager* GetInst()
		{
			if (!_inst)
			{
				_inst = new DataManager();
			}
			return _inst;
		}

		~DataManager()
		{
			if (_inst)
			{
				delete _inst;
			}
			_inst = nullptr;
		}

	public:
		void LoadData()
		{
			_statMap = StatData::GetInst()->MakeDict(ConfigManager::GetInst()->GetServerConfig().dataPath + "\\StatData.json");
			_skillMap = SkillData::GetInst()->MakeDict(ConfigManager::GetInst()->GetServerConfig().dataPath + "\\SkillData.json");
		}

		map<int32, Protocol::StatInfo*> GetStatMap() { return _statMap; }
		map<int32, Skill*> GetSkillMap() { return _skillMap; }

	private : 
		static DataManager* _inst;
		map<int32, Protocol::StatInfo*> _statMap;
		map<int32, Skill*> _skillMap;
	};
}

