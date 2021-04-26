/* This file is auto generated using ArgSpawn */

#pragma once

#include <string>

namespace argspawn {
	struct Arguments {
		std::string YamlPath;
		std::string OutDir;
		bool Verbose;
	};

	class ArgParser {
	private:
		enum class States {
			Default = 0,
			OutDir
		};

		States state;
		Arguments arguments;
		int m_Argc;
		char** m_Argv;

	public:
		ArgParser() = delete;
		ArgParser(int argc, char** argv);
		Arguments ParseArguments();

	private:
		void PrintHelpTextAndExit() const;
	};
}