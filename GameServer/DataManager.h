#pragma once

#include "pch.h"
#include "Contents.h"
#include "ConfigManager.h"

namespace FrokEngine
{
	class DataManager
	{
	public:
		static map<int32, Protocol::StatInfo> StatDict{ get; private set; } = new Dictionary<int, StatInfo>();
		static map<int32, Data::Skill> SkillDict{ get; private set; } = new Dictionary<int, Data.Skill>();

		static void LoadData()
		{
			StatDict = LoadJson<Data.StatData, int, StatInfo>("StatData").MakeDict();
			SkillDict = LoadJson<Data.SkillData, int, Data.Skill>("SkillData").MakeDict();
		}

		static Loader LoadJson<Loader, Key, Value>(string path) where Loader : ILoader<Key, Value>
		{
			string text = File.ReadAllText($"{ConfigManager.Config.dataPath}/{path}.json");
			return Newtonsoft.Json.JsonConvert.DeserializeObject<Loader>(text);
		}
	};
}

