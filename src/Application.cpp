#include "fmt/ranges.h"

#include "frontend/YamlFrontend.h"

static void PrintIntermediateRepresentation(IntermediateRepresentation& irep);

int main()
{
	try
	{
		IntermediateRepresentation irep{ YamlFrontendParser::ParseYamlFromFile("./yamls/Example1.yaml") };
		PrintIntermediateRepresentation(irep);
	}
	catch (const YAML::Exception& e)
	{
		fmt::print("[Error] Failed to Parse\n[Error Msg] {0}\n", e.msg);
	}
	catch (const std::exception& e)
	{
		fmt::print("[Error] Failed to Parse\n[Error Msg] {0}\n", e.what());
	}
	return 0;
}

static void PrintIntermediateRepresentation(IntermediateRepresentation& irep)
{
	fmt::print("Program Description: {0}\n", irep.GetProgramDescription());
	fmt::print("Angle Includes: {0}\n", fmt::join(irep.GetAngleIncludesRef(), " and "));
	fmt::print("Quote Includes: {0}\n", fmt::join(irep.GetQuoteIncludesRef(), " and "));
	fmt::print("Namespace Name: {0}\n", irep.GetNamespaceName());
	fmt::print("Class Name: {0}\n", irep.GetClassName());

	fmt::print("> Positional Args\n");
	for (auto& pa : irep.GetPositionalArgumentsRef())
	{
		fmt::print("- Name: {0}\n", pa.GetName());
		fmt::print("  Description: {0}\n", pa.GetDescription());
		fmt::print("  Type: {0}\n", pa.GetType());
		fmt::print("  Conversion: {0}\n", pa.GetConversion());
	}

	fmt::print("> Optional Args\n");
	for (auto& oa : irep.GetOptionalArgumentsRef())
	{
		fmt::print("- Name: {0}\n", oa.GetName());
		fmt::print("  Description: {0}\n", oa.GetDescription());
		fmt::print("  Type: {0}\n", oa.GetType());
		fmt::print("  Conversion: {0}\n", oa.GetConversion());
		fmt::print("  Symbols: {0}\n", fmt::join(oa.GetSymbols(), " or "));
		fmt::print("  Default Value: {0}\n", oa.GetDefaultValue());
	}

	fmt::print("> Flags\n");
	for (auto& fg : irep.GetFlagsRef())
	{
		fmt::print("- Name: {0}\n", fg.GetName());
		fmt::print("  Description: {0}\n", fg.GetDescription());
		fmt::print("  Type: {0}\n", fg.GetType());
		fmt::print("  Conversion: {0}\n", fg.GetConversion());
		fmt::print("  Symbols: {0}\n", fmt::join(fg.GetSymbols(), " or "));
	}
}
