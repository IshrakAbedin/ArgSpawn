#pragma once

#include <tuple>

#include "intermediate/IntermediateRepresentation.h"

class CppGenerator
{
private:
	static std::string GenerateCppHeader(IntermediateRepresentation& irep);
	static std::string GenerateCppBody(IntermediateRepresentation& irep);
public:
	static std::tuple<std::string, std::string> GenerateCppHeaderAndBody(IntermediateRepresentation& irep);
};