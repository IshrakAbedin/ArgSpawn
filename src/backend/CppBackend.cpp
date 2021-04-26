#include "CppBackend.h"

#include "fmt/ranges.h"

#include "CppFormatTemplates.h" // Highly dependent on the macros and constexprs

constexpr auto RETURN_STRUCT_TNAME = "Arguments";
constexpr auto STATE_ENUMCLASS_TNAME = "States";
constexpr auto RETURN_STRUCT_VAR_NAME = "arguments";
constexpr auto STATE_ENUMCLASS_VAR_NAME = "state";
constexpr auto PARSING_FUNC_NAME = "ParseArguments";
constexpr auto HELPTEXT_FUNC_NAME = "PrintHelpTextAndExit";
constexpr auto STATE_ENUM_DEFAULT = "Default";
constexpr auto VAR_PARSE_COUNT_NAME = "parseCount";
constexpr auto VAR_TARGET_COUNT_NAME = "targetCount";
constexpr auto VAR_MAX_ARG_NAME = "maxArgCount";
constexpr auto VAR_EACH_ARG_NAME = "arg";
constexpr auto HELP_TEXT_MSG = "Print help message and exit";
constexpr auto POSITIONAL_ARG_OVERFLOW_MSG = "[!ERROR] Provided more positional arguments than allowed";
constexpr auto POSITIONAL_ARG_UNDERFLOW_MSG = "[!ERROR] Provided less positional arguments than required";

constexpr auto UTILFUNC_STRING_HASH_NAME = "Djb2";
constexpr auto UTILFUNC_STRING_HASH = "static constexpr unsigned int Djb2(const char* str, int h = 0)\n"
	"{\n return !str[h] ? 5381 : (Djb2(str, h + 1) * 33) ^ str[h];\n}";

const std::vector<std::string> VEC_HELP_SYMBOLS = { "-h", "--help" };

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
	ACC_DECLARE_VAR(STATE_ENUMCLASS_TNAME, STATE_ENUMCLASS_VAR_NAME);
	ACC_DECLARE_VAR(RETURN_STRUCT_TNAME, RETURN_STRUCT_VAR_NAME);
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
	int counter;

	// Add class header inclusion
	ACC_QHEADER_INCLUDE(fmt::format("{0}.h", irep.GetClassName()));
	ACC_NEWLINE;
	
	// Add iostream for external library independent printing and string for string type
	ACC_AHEADER_INCLUDE("iostream");
	// ACC_AHEADER_INCLUDE("string");
	ACC_NEWLINE;

	// Push namespace if it is named
	if (irep.GetNamespaceName() != "") ACC_NAMESPACE_BEGIN(irep.GetNamespaceName());

	// Push the utility constexpr string hashing function
	ACCPB(UTILFUNC_STRING_HASH);
	ACC_NEWLINE;

	/*-------------------------- PARAMETERIZED CONSTRUCTOR BEGINS --------------------------*/
	ACC_DEFINE_PARAMCTOR(irep.GetClassName(), "int argc, char** argv");
	ACCPB_FMT("{0} = {1}::{2};", STATE_ENUMCLASS_VAR_NAME, STATE_ENUMCLASS_TNAME, STATE_ENUM_DEFAULT); // Set state to default
	if (!irep.GetOptionalArgumentsRef().empty()) // Initialize optional variable defaults
	{
		for (auto& arg : irep.GetOptionalArgumentsRef())
		{
			ACCPB_FMT("{0}.{1} = {2};", RETURN_STRUCT_VAR_NAME, arg.GetName(), arg.GetDefaultValue());
		}
	}
	if (!irep.GetFlagsRef().empty()) // Initialize flags as false
	{
		for (auto& arg : irep.GetFlagsRef())
		{
			ACCPB_FMT("{0}.{1} = {2};", RETURN_STRUCT_VAR_NAME, arg.GetName(), "false");
		}
	}
	ACCPB("m_Argc = argc;"); // Set argc value
	ACCPB("m_Argv = argv;"); // Set argv value
	ACC_FUNC_END;
	/*-------------------------- PARAMETERIZED CONSTRUCTOR ENDS --------------------------*/
	ACC_NEWLINE;

	/*-------------------------- PARSE ARGUMENT FUNCTION BEGINS --------------------------*/
	ACC_DEFINE_CLASSFUNC(RETURN_STRUCT_TNAME, irep.GetClassName(), PARSING_FUNC_NAME, "", "");
	
	// Control variables for argument counting
	ACC_DECDEFINE_VAR("int", VAR_PARSE_COUNT_NAME, 0);
	ACC_DECDEFINE_VAR("int", VAR_TARGET_COUNT_NAME, irep.GetPositionalArgumentsRef().size());
	ACC_DECDEFINE_VAR("int", VAR_MAX_ARG_NAME, irep.GetPositionalArgumentsRef().size() +
		irep.GetOptionalArgumentsRef().size() + irep.GetFlagsRef().size());
	ACC_NEWLINE;

	// Initial check to match argument count
	ACC_IF(fmt::format("m_Argc < {0} + 1", VAR_TARGET_COUNT_NAME));
	ACC_COUTLN(POSITIONAL_ARG_UNDERFLOW_MSG);
	ACC_COUT_NEWLINE;
	ACC_FUNC_CALL(HELPTEXT_FUNC_NAME, "");
	ACC_IF_END;
	ACC_NEWLINE;

	// Main loop through each of the arguments
	ACC_FOR("int i = 1", "i < m_Argc", "i++");
	ACC_DECDEFINE_VAR("auto", VAR_EACH_ARG_NAME, "m_Argv[i]");
	ACC_NEWLINE;

	/* Level 0 Indent Switch Starts*/
	ACC_SWITCH(UTIL_FUNC_CALL(UTILFUNC_STRING_HASH_NAME, VAR_EACH_ARG_NAME));

	for (auto& helpText : VEC_HELP_SYMBOLS)
	{
		ACC_CASE(UTIL_FUNC_CALL(UTILFUNC_STRING_HASH_NAME, UTIL_STR(helpText)));
	}
	ACC_FUNC_CALL(HELPTEXT_FUNC_NAME, "");
	ACC_BREAK;
	for (auto& arg : irep.GetOptionalArgumentsRef())
	{
		for (auto& symbol : arg.GetSymbols())
		{
			ACC_CASE(UTIL_FUNC_CALL(UTILFUNC_STRING_HASH_NAME, UTIL_STR(symbol)));
		}
		ACC_DEFINE_VAR(STATE_ENUMCLASS_VAR_NAME, UTIL_STATIC_MEMBER(STATE_ENUMCLASS_TNAME, arg.GetName()));
		ACC_BREAK;
	}
	for (auto& arg : irep.GetFlagsRef())
	{
		for (auto& symbol : arg.GetSymbols())
		{
			ACC_CASE(UTIL_FUNC_CALL(UTILFUNC_STRING_HASH_NAME, UTIL_STR(symbol)));
		}
		ACC_DEFINE_VAR(UTIL_OBJECT_MEMBER(RETURN_STRUCT_VAR_NAME, arg.GetName()), "true");
		ACC_BREAK;
	}
	ACC_CASE_DEFAULT;
	/* Level 1 Indent Switch Starts */
	ACC_SWITCH(STATE_ENUMCLASS_VAR_NAME);
	
	ACC_CASE(UTIL_STATIC_MEMBER(STATE_ENUMCLASS_TNAME, STATE_ENUM_DEFAULT));
	/* Level 2 Indent Switch Starts */

	ACC_SWITCH(VAR_PARSE_COUNT_NAME);
	counter = 0;
	for (auto& arg : irep.GetPositionalArgumentsRef())
	{
		ACC_CASE(counter);
		if (arg.GetConversion() != "")
		{
			ACC_DEFINE_VAR(UTIL_OBJECT_MEMBER(RETURN_STRUCT_VAR_NAME, arg.GetName()),
				UTIL_FUNC_CALL(arg.GetConversion(), VAR_EACH_ARG_NAME));
		}
		else
		{
			ACC_DEFINE_VAR(UTIL_OBJECT_MEMBER(RETURN_STRUCT_VAR_NAME, arg.GetName()), VAR_EACH_ARG_NAME);
		}
		ACC_BREAK;
		counter++;
	}
	ACC_CASE_DEFAULT;
	ACC_COUTLN(POSITIONAL_ARG_OVERFLOW_MSG);
	ACC_COUT_NEWLINE;
	ACC_FUNC_CALL(HELPTEXT_FUNC_NAME, "");
	ACC_BREAK;

	ACC_SWITCH_END;
	/* Level 2 Indent Switch Ends */
	ACC_INCREMENT_VAR(VAR_PARSE_COUNT_NAME);
	ACC_BREAK;

	for (auto& arg : irep.GetOptionalArgumentsRef())
	{
		ACC_CASE(UTIL_STATIC_MEMBER(STATE_ENUMCLASS_TNAME, arg.GetName()));
		if (arg.GetConversion() != "")
		{
			ACC_DEFINE_VAR(UTIL_OBJECT_MEMBER(RETURN_STRUCT_VAR_NAME, arg.GetName()),
				UTIL_FUNC_CALL(arg.GetConversion(), VAR_EACH_ARG_NAME));
		}
		else
		{
			ACC_DEFINE_VAR(UTIL_OBJECT_MEMBER(RETURN_STRUCT_VAR_NAME, arg.GetName()), VAR_EACH_ARG_NAME);
		}
		ACC_DEFINE_VAR(STATE_ENUMCLASS_VAR_NAME, UTIL_STATIC_MEMBER(STATE_ENUMCLASS_TNAME, STATE_ENUM_DEFAULT));
		ACC_BREAK;
	}
	ACC_CASE_DEFAULT;
	// A execution should never reach here
	ACC_FUNC_CALL(HELPTEXT_FUNC_NAME, "");
	ACC_BREAK;
	ACC_SWITCH_END;
	/* Leve1 Indent Switch Ends */
	ACC_BREAK;

	ACC_SWITCH_END;
	/* Level 0 Indent Switch Ends */
	ACC_FOR_END;

	// Exit if argument count underflows
	ACC_IF(fmt::format("{0} != {1}", VAR_PARSE_COUNT_NAME, VAR_TARGET_COUNT_NAME));
	ACC_COUTLN(POSITIONAL_ARG_UNDERFLOW_MSG);
	ACC_COUT_NEWLINE;
	ACC_FUNC_CALL(HELPTEXT_FUNC_NAME, "");
	ACC_IF_END;
	ACC_NEWLINE;
	ACC_RETURN(RETURN_STRUCT_VAR_NAME);

	ACC_FUNC_END;
	/*-------------------------- PARSE ARGUMENT FUNCTION ENDS --------------------------*/
	ACC_NEWLINE;

	/*-------------------------- HELP TEXT FUNCTION BEGINS --------------------------*/
	ACC_DEFINE_CLASSFUNC("void", irep.GetClassName(), HELPTEXT_FUNC_NAME, "", "const");
	ACC_COUTLN(irep.GetProgramDescription());
	ACC_COUT_NEWLINE;

	auto desc = fmt::format("[{0}] : {1}", fmt::join(VEC_HELP_SYMBOLS, ", "), HELP_TEXT_MSG);
	ACC_COUTLN(desc);
	ACC_COUT_NEWLINE;

	if (!irep.GetPositionalArgumentsRef().empty())
	{
		ACC_COUTLN("POSITIONAL ARGUMENTS\\n--------------------");
		for (auto& arg : irep.GetPositionalArgumentsRef())
		{
			auto desc = fmt::format("[{0}] : {1}", arg.GetName(), arg.GetDescription());
			ACC_COUTLN(desc);
		}
	}
	ACC_COUT_NEWLINE;
	if (!irep.GetOptionalArgumentsRef().empty())
	{
		ACC_COUTLN("OPTIONAL ARGUMENTS\\n--------------------");
		for (auto& arg : irep.GetOptionalArgumentsRef())
		{
			auto desc = fmt::format("[{0}] : {1}", fmt::join(arg.GetSymbols(), ", "), arg.GetDescription());
			ACC_COUTLN(desc);
		}
	}
	ACC_COUT_NEWLINE;
	if (!irep.GetFlagsRef().empty())
	{
		ACC_COUTLN("Flags\\n-----");
		for (auto& arg : irep.GetFlagsRef())
		{
			auto desc = fmt::format("[{0}] : {1}", fmt::join(arg.GetSymbols(), ", "), arg.GetDescription());
			ACC_COUTLN(desc);
		}
	}
	ACC_COUT_NEWLINE;
	ACC_EXIT(0);
	ACC_FUNC_END;
	/*-------------------------- HELP TEXT FUNCTION ENDS --------------------------*/

	if (irep.GetNamespaceName() != "") ACC_NAMESPACE_END;

	return fmt::format("{0}", fmt::join(ACC_NAME, "\n"));
}

std::tuple<std::string, std::string> CppGenerator::GenerateCppHeaderAndBody(IntermediateRepresentation& irep)
{
	return { GenerateCppHeader(irep), GenerateCppBody(irep) };
}
