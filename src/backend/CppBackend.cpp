#include "CppBackend.h"

#include "fmt/ranges.h"

#include "CppFormatTemplates.h" // Highly dependent on the macors and contexprs

constexpr auto RETURN_STRUCT_TNAME = "Arguments";
constexpr auto STATE_ENUMCLASS_TNAME = "States";
constexpr auto RETURN_STRUCT_NAME = "arguments";
constexpr auto STATE_ENUMCLASS_NAME = "states";
constexpr auto PARSING_FUNC_NAME = "Parse Arguments";
constexpr auto HELPTEXT_FUNC_NAME = "PrintHelpTextAndExit";

std::string CppGenerator::GenerateCppHeader(IntermediateRepresentation& irep)
{
	std::vector<std::string> ACC_NAME;

	ACCPB(TMPLT_PRAGMA_0); // push pragma once
	ACC_NEWLINE; // newline

	for (auto& angleInclude : irep.GetAngleIncludesRef()) // push angle includes
	{
		ACCPB_FMT(TMPLT_ANGLE_INCLUDE_1, angleInclude);
	}
	ACC_NEWLINE;

	for (auto& quoteInclude : irep.GetQuoteIncludesRef()) // push quote includes
	{
		ACCPB_FMT(TMPLT_QUOTE_INCLUDE_1, quoteInclude);
	}
	ACC_NEWLINE;

	// Prepare the return struct
	ACC_STRUCT_BEGIN(RETURN_STRUCT_TNAME);
	for (auto& arg : irep.GetPositionalArgumentsRef()) 
	{
		ACC_DECLAREVAR(arg.GetType(), arg.GetName());
	}
	for (auto& arg : irep.GetOptionalArgumentsRef())
	{
		ACC_DECLAREVAR(arg.GetType(), arg.GetName());
	}
	for (auto& arg : irep.GetFlagsRef())
	{
		ACC_DECLAREVAR(arg.GetType(), arg.GetName());
	}
	ACC_STRUCT_END;
	ACC_NEWLINE;

	// Prepare the parser class
	ACC_CLASS_BEGIN(irep.GetClassName());

	ACC_PRIVATE;

	// Prepare enum for state switching
	ACC_ENUMCLASS_BEGIN(STATE_ENUMCLASS_TNAME);
	irep.GetOptionalArgumentsRef().empty() ? ACCPB("Default = 0,") : ACCPB("Default = 0");
	if (!irep.GetOptionalArgumentsRef().empty())
	{
		std::vector<std::string> optArgNames;
		for (auto& arg : irep.GetOptionalArgumentsRef())
		{
			optArgNames.push_back(arg.GetName());
		}
		ACCPB_FMT("{0}", fmt::join(optArgNames, ",\n"));
	}
	ACC_ENUMCLASS_END;
	ACC_NEWLINE;

	// Add the other necessary variables
	ACC_DECLAREVAR(STATE_ENUMCLASS_TNAME, STATE_ENUMCLASS_NAME);
	ACC_DECLAREVAR(RETURN_STRUCT_TNAME, RETURN_STRUCT_NAME);
	ACC_DECLAREVAR("int", "m_Argc"); // Argument count
	ACC_DECLAREVAR("char**", "m_Argv"); // Argument values
	ACC_NEWLINE;

	// Declare constructors and methods
	ACC_PUBLIC;
	ACC_DECLARE_CTOR(irep.GetClassName(), "= delete"); // Default ctor deleted
	ACC_DECLARE_PARAMCTOR(irep.GetClassName(), "int argc, char** argv");
	ACC_DECLAREFUNC(RETURN_STRUCT_NAME, PARSING_FUNC_NAME, "");
	ACC_NEWLINE;

	ACC_PRIVATE;
	ACC_DECLAREFUNC("void", HELPTEXT_FUNC_NAME, "const");

	ACC_CLASS_END;
	ACC_NEWLINE;

	return fmt::format("{0}", fmt::join(accumulator, "\n"));
}

std::string CppGenerator::GenerateCppBody(IntermediateRepresentation& irep)
{
	return std::string();
}

std::tuple<std::string, std::string> CppGenerator::GenerateCppHeaderAndBody(IntermediateRepresentation& irep)
{
	return { GenerateCppHeader(irep), GenerateCppBody(irep) };
}
