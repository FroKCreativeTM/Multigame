#pragma once

#include "pch.h"

#include "Protocol.pb.h"
#include "Enum.pb.h"
#include "Struct.pb.h"

namespace FrokEngine
{
	namespace Data
	{
		class ProjectileInfo
		{
		public:
			string name;
			float speed;
			int range;
			string prefab;
		};

		class Skill
		{
		public:
			int id;
			string name;
			float cooldown;
			int damage;
			Protocol::SkillType skillType;
			shared_ptr<ProjectileInfo> projectile;
		};

		class SkillData
		{
		public : 
			static map<int, shared_ptr<Skill>> MakeDict(string dataPath)
			{
				// TODO : json �Ľ��ؼ� ������ ��Ƴֱ�

				map<int, shared_ptr<Skill>> m;
				for (Skill skill : skills)
					m.insert(make_pair(skill.id, make_shared<Skill>(skill)));
				return m;
			}

		private : 
			static list<Skill> skills;
		};

		class StatData
		{
		public:
			static map<int32, shared_ptr<Protocol::StatInfo>> MakeDict(string dataPath)
			{
				// TODO : json �Ľ��ؼ� ������ ��Ƴֱ�
				std::ifstream fin;
				fin.open("config.json", std::ios::in);
				std::string str;
				str.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

				Json::Reader reader;
				Json::Value root;
				if (reader.parse(str, root) == false) {
					std::cerr << "Failed to parse Json : " << reader.getFormattedErrorMessages() << std::endl;
					// ��� ������
					return map<int32, shared_ptr<Protocol::StatInfo>>();
				}

				// ���� �� �� ����

				map<int32, shared_ptr<Protocol::StatInfo>> m;
				for (auto stat : stats)
				{
					stat.set_hp(stat.maxhp());
					m.insert(make_pair(stat.level(), make_shared<Protocol::StatInfo>(stat)));
				}
				return m;
			}

		private:
			static list<Protocol::StatInfo> stats;
		};
	}
}