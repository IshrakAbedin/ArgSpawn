/* This file is auto generated using ArgSpawn, then modified to switch iostream to fmt */

#include "ArgParser.h"

#include "fmt/core.h"

namespace argspawn
{
	static constexpr unsigned int Djb2(const char* str, int h = 0)
	{
		return !str[h] ? 5381 : (Djb2(str, h + 1) * 33) ^ str[h];
	}

	ArgParser::ArgParser(int argc, char** argv)
	{
		state = States::Default;
		arguments.OutDir = "./";
		arguments.Verbose = false;
		m_Argc = argc;
		m_Argv = argv;
	}

	Arguments ArgParser::ParseArguments()
	{
		int parseCount = 0;
		int targetCount = 1;

		for (int i = 1; i < m_Argc; i++)
		{
			auto arg = m_Argv[i];

			switch (Djb2(arg))
			{
			case Djb2("-h"):
			case Djb2("--help"):
				PrintHelpTextAndExit();
				break;
			case Djb2("-d"):
			case Djb2("--dir"):
				state = States::OutDir;
				break;
			case Djb2("-v"):
			case Djb2("--verbose"):
				arguments.Verbose = true;
				break;
			default:
				switch (state)
				{
				case States::Default:
					switch (parseCount)
					{
					case 0:
						arguments.YamlPath = arg;
						break;
					default:
						fmt::print("[!ERROR] Provided more positional arguments than allowed\n");
						fmt::print("\n");
						PrintHelpTextAndExit();
						break;
					}
					parseCount++;
					break;
				case States::OutDir:
					arguments.OutDir = arg;
					state = States::Default;
					break;
				default:
					PrintHelpTextAndExit();
					break;
				}
				break;
			}
		}
		if (parseCount != targetCount)
		{
			fmt::print("[!ERROR] Provided less positional arguments than required\n");
			fmt::print("\n");
			PrintHelpTextAndExit();
		}

		return arguments;
	}

	void ArgParser::PrintHelpTextAndExit() const
	{
		fmt::print("ArgSpawn || A command line argument parser generator\n----------------------------------------------------\nMohammad Ishrak Abedin, 2021\n");
		fmt::print("\n");
		fmt::print("[-h, --help] : Print help message and exit\n");
		fmt::print("\n");
		fmt::print("POSITIONAL ARGUMENTS\n--------------------\n");
		fmt::print("[YamlPath] : Path to yaml config file\n");
		fmt::print("\n");
		fmt::print("OPTIONAL ARGUMENTS\n--------------------\n");
		fmt::print("[-d, --dir] : Output directory for the generated files (default is execution directory)\n");
		fmt::print("\n");
		fmt::print("Flags\n-----\n");
		fmt::print("[-v, --verbose] : Print the generated files in console\n");
		fmt::print("\n");
		exit(0);
	}
}