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
			_statDict = LoadJson<int, Protocol::StatInfo>("StatData").MakeDict();
			_skillDict = LoadJson<int, Data::Skill>("SkillData").MakeDict();
		}

		template <typename Key, typename Value>
		static map<Key, Value> LoadJson(string path)
		{
			string text = File.ReadAllText($"{ConfigManager.Config.dataPath}/{path}.json");
			return Newtonsoft.Json.JsonConvert.DeserializeObject<Loader>(text);
		}

	private : 
		static map<int32, Protocol::StatInfo> _statDict;
		static map<int32, Data::Skill> _skillDict;
	};
}

