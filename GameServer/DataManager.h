#pragma once

#include "pch.h"
#include "Contents.h"
#include "ConfigManager.h"

namespace FrokEngine
{
	class DataManager
	{
	public:
		static void LoadData()
		{
			_statMap = Data::StatData::MakeDict(ConfigManager::GetServerConfig().dataPath + "\\StatData.json");
			_skillMap = Data::SkillData::MakeDict(ConfigManager::GetServerConfig().dataPath + "\\SkillData.json");
		}

		static map<int32, shared_ptr<Protocol::StatInfo>> GetStatMap() { return _statMap; }
		static map<int32, shared_ptr<Data::Skill>> GetSkillMap() { return _skillMap; }

	private : 
		static map<int32, shared_ptr<Protocol::StatInfo>> _statMap;
		static map<int32, shared_ptr<Data::Skill>> _skillMap;
	};
}

