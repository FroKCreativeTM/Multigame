#pragma once

#include "pch.h"

namespace FrokEngine
{
	class ServerConfig
	{
	public:
		string dataPath;
	};

	class ConfigManager
	{
	public:
		static void LoadConfig()
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
		}

		ServerConfig GetServerConfig() const { return _config; }
	private:
		static ServerConfig _config;
	};
}
