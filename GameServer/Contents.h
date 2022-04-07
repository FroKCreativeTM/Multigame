#pragma once

#include "pch.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

namespace FrokEngine
{
	namespace Data
	{
		class StatData 
		{
			list<Protocol::StatInfo> stats;

			map<int32, Protocol::StatInfo> MakeDict()
			{
				map<int32, Protocol::StatInfo> m;
				for (auto stat : stats)
				{
					stat.set_hp(stat.maxhp());
					m.insert(make_pair(stat.level(), stat));
				}
				return m;
			}
		};

		class Skill
		{
		public:
			int id;
			string name;
			float cooldown;
			int damage;
			Protocol::SkillType skillType;
			ProjectileInfo projectile;
		};

		class ProjectileInfo
		{
		public:
			string name;
			float speed;
			int range;
			string prefab;
		};

		class SkillData
		{
			List<Skill> skills;

			map<int, Skill> MakeDict()
			{
				map<int, Skill> m;
				for (Skill skill : skills)
					m.insert(make_pair(skill.id, skill));
				return m;
			}
		};
	}
}