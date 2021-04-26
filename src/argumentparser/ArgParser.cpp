/* This file is auto generated using ArgSpawn */

#include "ArgParser.h"

#include <iostream>

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
		int maxArgCount = 3;

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
						std::cout << "[!ERROR] Provided more positional arguments than allowed" << std::endl;
						std::cout << std::endl;
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
			std::cout << "[!ERROR] Provided less positional arguments than required" << std::endl;
			std::cout << std::endl;
			PrintHelpTextAndExit();
		}

		return arguments;
	}

	void ArgParser::PrintHelpTextAndExit() const
	{
		std::cout << "ArgSpawn || A command line argument parser generator\n----------------------------------------------------\nMohammad Ishrak Abedin, 2021" << std::endl;
		std::cout << std::endl;
		std::cout << "[-h, --help] : Print help message and exit" << std::endl;
		std::cout << std::endl;
		std::cout << "POSITIONAL ARGUMENTS\n--------------------" << std::endl;
		std::cout << "[YamlPath] : Path to yaml config file" << std::endl;
		std::cout << std::endl;
		std::cout << "OPTIONAL ARGUMENTS\n--------------------" << std::endl;
		std::cout << "[-d, --dir] : Output directory for the generated files (default is execution directory)" << std::endl;
		std::cout << std::endl;
		std::cout << "Flags\n-----" << std::endl;
		std::cout << "[-v, --verbose] : Print the generated files in console" << std::endl;
		std::cout << std::endl;
		exit(0);
	}
}