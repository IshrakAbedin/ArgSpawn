#include "CppBackend.h"

#include "fmt/ranges.h"

#include "CppFormatTemplates.h" // Highly dependent on the macors and contexprs

constexpr auto RETURN_STRUCT_TNAME = "Arguments";
constexpr auto STATE_ENUMCLASS_TNAME = "States";
constexpr auto RETURN_STRUCT_NAME = "arguments";
constexpr auto STATE_ENUMCLASS_NAME = "states";
constexpr auto PARSING_FUNC_NAME = "ParseArguments";
constexpr auto HELPTEXT_FUNC_NAME = "PrintHelpTextAndExit";
constexpr auto STATE_ENUM_DEFAULT = "Default";

std::string CppGenerator::GenerateCppHeader(IntermediateRepresentation& irep)
{
	std::vector<std::string> ACC_NAME;

	ACCPB(TMPLT_PRAGMA_0); // Push pragma once
	ACC_NEWLINE; // Push newline

	// Push angle includes
	for (auto& angleInclude : irep.GetAngleIncludesRef())
	{
		ACC_AHEADER_INCLUDE(angleInclude);
	}
	ACC_NEWLINE;

	// Push quote includes
	for (auto& quoteInclude : irep.GetQuoteIncludesRef())
	{
		ACC_QHEADER_INCLUDE(quoteInclude);
	}
	ACC_NEWLINE;

	// Push namespace if it is named
	if (irep.GetNamespaceName() != "") ACC_NAMESPACE_BEGIN(irep.GetNamespaceName());

	// Prepare the return struct
	ACC_STRUCT_BEGIN(RETURN_STRUCT_TNAME);
	for (auto& arg : irep.GetPositionalArgumentsRef()) 
	{
		ACC_DECLARE_VAR(arg.GetType(), arg.GetName());
	}
	for (auto& arg : irep.GetOptionalArgumentsRef())
	{
		ACC_DECLARE_VAR(arg.GetType(), arg.GetName());
	}
	for (auto& arg : irep.GetFlagsRef())
	{
		ACC_DECLARE_VAR(arg.GetType(), arg.GetName());
	}
	ACC_STRUCT_END;
	ACC_NEWLINE;

	// Prepare the parser class
	ACC_CLASS_BEGIN(irep.GetClassName());

	ACC_PRIVATE;

	// Prepare enum for state switching
	ACC_ENUMCLASS_BEGIN(STATE_ENUMCLASS_TNAME);
	irep.GetOptionalArgumentsRef().empty() ? ACCPB_FMT("{0} = 0", STATE_ENUM_DEFAULT) : ACCPB_FMT("{0} = 0,", STATE_ENUM_DEFAULT);
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
	ACC_DECLARE_VAR(STATE_ENUMCLASS_TNAME, STATE_ENUMCLASS_NAME);
	ACC_DECLARE_VAR(RETURN_STRUCT_TNAME, RETURN_STRUCT_NAME);
	ACC_DECLARE_VAR("int", "m_Argc"); // Argument count
	ACC_DECLARE_VAR("char**", "m_Argv"); // Argument values
	ACC_NEWLINE;

	// Declare constructors and methods
	ACC_PUBLIC;
	ACC_DECLARE_CTOR(irep.GetClassName(), "= delete"); // Default ctor deleted
	ACC_DECLARE_PARAMCTOR(irep.GetClassName(), "int argc, char** argv");
	ACC_DECLARE_FUNC(RETURN_STRUCT_TNAME, PARSING_FUNC_NAME, "", "");
	ACC_NEWLINE;

	ACC_PRIVATE;
	ACC_DECLARE_FUNC("void", HELPTEXT_FUNC_NAME, "", "const");

	ACC_CLASS_END;

	if (irep.GetNamespaceName() != "") ACC_NAMESPACE_END;

	return fmt::format("{0}", fmt::join(ACC_NAME, "\n"));
}

std::string CppGenerator::GenerateCppBody(IntermediateRepresentation& irep)
{
	std::vector<std::string> ACC_NAME;

	// Add class header inclusion
	ACC_QHEADER_INCLUDE(fmt::format("{0}.h", irep.GetClassName()));
	ACC_NEWLINE;
	
	// Add iostream for external library independent printing
	ACC_AHEADER_INCLUDE("iostream");
	ACC_NEWLINE;

	// Push namespace if it is named
	if (irep.GetNamespaceName() != "") ACC_NAMESPACE_BEGIN(irep.GetNamespaceName());

	// Push ParamCtor
	ACC_DEFINE_PARAMCTOR(irep.GetClassName(), "int argc, char** argv");
	ACCPB_FMT("{0} = {1}::{2};", STATE_ENUMCLASS_NAME, STATE_ENUMCLASS_TNAME, STATE_ENUM_DEFAULT); // Set state to default
	if (!irep.GetOptionalArgumentsRef().empty()) // Initialize optional variable defaults
	{
		for (auto& arg : irep.GetOptionalArgumentsRef())
		{
			ACCPB_FMT("{0}.{1} = {2};", RETURN_STRUCT_NAME, arg.GetName(), arg.GetDefaultValue());
		}
	}
	if (!irep.GetFlagsRef().empty()) // Initialize flags as false
	{
		for (auto& arg : irep.GetFlagsRef())
		{
			ACCPB_FMT("{0}.{1} = {2};", RETURN_STRUCT_NAME, arg.GetName(), "false");
		}
	}
	ACCPB("m_Argc = argc;"); // Set argc value
	ACCPB("m_Argv = argv;"); // Set argv value
	ACC_FUNC_END;
	ACC_NEWLINE;

	ACC_DEFINE_CLASSFUNC(RETURN_STRUCT_TNAME, irep.GetClassName(), PARSING_FUNC_NAME, "", "");
	// TODO: Actual parser function
	ACC_FUNC_END;
	ACC_NEWLINE;

	// Help Text function
	ACC_DEFINE_CLASSFUNC("void", irep.GetClassName(), HELPTEXT_FUNC_NAME, "", "const");
	ACC_COUTLN(irep.GetProgramDescription());
	ACC_COUT_NEWLINE;
	if (!irep.GetPositionalArgumentsRef().empty())
	{
		ACC_COUTLN("POSITIONAL ARGUMENTS\\n--------------------");
		for (auto& arg : irep.GetPositionalArgumentsRef())
		{
			auto desc = fmt::format("[{0}] > {1}", arg.GetName(), arg.GetDescription());
			ACC_COUTLN(desc);
		}
	}
	if (!irep.GetOptionalArgumentsRef().empty())
	{
		ACC_COUTLN("OPTIONAL ARGUMENTS\\n--------------------");
		for (auto& arg : irep.GetOptionalArgumentsRef())
		{
			auto desc = fmt::format("[{0}] > {1}", fmt::join(arg.GetSymbols(), ", "), arg.GetDescription());
			ACC_COUTLN(desc);
		}
	}
	if (!irep.GetFlagsRef().empty())
	{
		ACC_COUTLN("Flags\\n-----");
		for (auto& arg : irep.GetFlagsRef())
		{
			auto desc = fmt::format("[{0}] > {1}", fmt::join(arg.GetSymbols(), ", "), arg.GetDescription());
			ACC_COUTLN(desc);
		}
	}
	ACC_FUNC_END;

	if (irep.GetNamespaceName() != "") ACC_NAMESPACE_END;

	return fmt::format("{0}", fmt::join(ACC_NAME, "\n"));
}

std::tuple<std::string, std::string> CppGenerator::GenerateCppHeaderAndBody(IntermediateRepresentation& irep)
{
	return { GenerateCppHeader(irep), GenerateCppBody(irep) };
}
