#pragma once

#include "YamlCppExtension.h"

class YamlFrontendParser
{
public:
	static IntermediateRepresentation ParseYamlFromFile(const std::string filePath);
};