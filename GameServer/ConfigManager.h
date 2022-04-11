#pragma once

#include "CorePch.h"

namespace FrokEngine
{
	class ServerConfig
	{
	public:
		string dataPath;
	};

	class ConfigManager
	{
	public : 
		static ConfigManager* GetInst()
		{
			if (!_inst)
			{
				_inst = new ConfigManager;
			}
			return _inst;
		}

		~ConfigManager()
		{
			if (_inst)
			{
				delete _inst;
			}
			_inst = nullptr;
		}

	public:
		void LoadConfig()
		{
			std::ifstream fin;
			fin.open("config.json", std::ios::in);
			std::string str;
			str.assign(std::istreambuf_iterator<char>(fin), std::istreambuf_iterator<char>());

			Json::Reader reader;
			Json::Value root;
			if (reader.parse(str, root) == false) {
				std::cerr << "Failed to parse Json : " << reader.getFormattedErrorMessages() << std::endl;
				return;
			}

			_config.dataPath = root["dataPath"].asString();

			fin.close();
		}

		ServerConfig GetServerConfig() { return _config; }
	private:
		static ConfigManager* _inst;
		ServerConfig _config;
	};
}
