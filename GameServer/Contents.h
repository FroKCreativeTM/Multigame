#pragma once

namespace FrokEngine
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
		ProjectileInfo projectile;
	};

	class SkillData
	{
	public:
		static SkillData* GetInst()
		{
			if (!_inst)
			{
				_inst = new SkillData();
			}
			return _inst;
		}

		~SkillData()
		{
			if (_inst)
			{
				delete _inst;
			}
			_inst = nullptr;
		}

	public:
		map<int, Skill*> MakeDict(string dataPath)
		{
			// TODO : json 파싱해서 데이터 모아넣기
			Json::Value root;
			Json::CharReaderBuilder reader;
			Json::String errorMessage;
			ifstream is(dataPath, ifstream::binary);
			auto bret = Json::parseFromStream(reader, is, &root, &errorMessage);

			if (bret == false) {
				cout << "Error to parse JSON file !!!" << endl;
			}

			auto jsonskills = root["skills"];

			for (auto skill : jsonskills)
			{
				Skill* s = new Skill();
				 
				s->id = stoi(skill["id"].asString());
				s->name =  skill["name"].asString();
				s->cooldown = stof(skill["cooldown"].asString());
				s->damage = stoi(skill["damage"].asString());

				if (skill["skillType"].asString() == "SkillAuto")
				{
					s->skillType = Protocol::SkillType::SKILL_AUTO;
				}
				else if (skill["skillType"].asString() == "SkillProjectile")
				{
					s->skillType = Protocol::SkillType::SKILL_PROJECTILE;
				}
				else
				{
					s->skillType = Protocol::SkillType::SKILL_NONE;
				}


				if (skill["projectile"].isObject())
				{
					auto proj = skill["projectile"];
					s->projectile.name = proj["name"].asString();
					s->projectile.speed = stof(proj["speed"].asString());
					s->projectile.range = stoi(proj["range"].asString());
					s->projectile.prefab = proj["prefab"].asString();
				}

				_skills.push_back(s);
			}

			is.close();

			map<int, Skill*> m;
			for (auto skill : _skills)
				m[skill->id] = skill;
			return m;
		}

	private:
		static SkillData* _inst;
		list<Skill*> _skills;
	};

	class StatData
	{
	public:
		static StatData* GetInst()
		{
			if (!_inst)
			{
				_inst = new StatData();
			}
			return _inst;
		}

		~StatData()
		{
			if (_inst)
			{
				delete _inst;
			}
			_inst = nullptr;
		}

	public:
		map<int32, Protocol::StatInfo*> MakeDict(string dataPath)
		{
			Json::Value root;
			Json::CharReaderBuilder reader;
			Json::String errorMessage;
			ifstream is(dataPath, ifstream::binary);
			auto bret = Json::parseFromStream(reader, is, &root, &errorMessage);

			if (bret == false) {
				cout << "Error to parse JSON file !!!" << endl;
			}

			auto stats = root["stats"];
			for (auto stat : stats)
			{
				auto s = new Protocol::StatInfo();

				s->set_level(stoi(stat["level"].asString()));
				s->set_maxhp(stoi(stat["maxHp"].asString()));
				s->set_attack(stoi(stat["attack"].asString()));
				s->set_speed(stof(stat["speed"].asString()));
				s->set_totalexp(stoi(stat["totalExp"].asString()));

				_stats.push_back(s);
			}

			is.close();

			map<int32, Protocol::StatInfo*> m;
			for (auto stat : _stats)
			{
				stat->set_hp(stat->maxhp());
				m[stat->level()] = stat;
			}
			return m;
		}

	private:
		static  StatData* _inst;
		list<Protocol::StatInfo*> _stats;
	};

}