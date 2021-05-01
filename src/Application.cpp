#include <filesystem>
#include <fstream>
#include "fmt/ranges.h"

#include "frontend/YamlFrontend.h"
#include "backend/CppBackend.h"
#include "argumentparser/ArgParser.h"

static void WriteFile(const std::string& path, const std::string& content) noexcept;
static void PrintHeaderAndBody(const std::string className, const std::string& header, const std::string& body) noexcept;
#ifdef DEBUG
static void PrintIntermediateRepresentation(IntermediateRepresentation& irep);
#endif // DEBUG

int main(int argc, char ** argv)
{
	try
	{
		auto parser = argspawn::ArgParser(argc, argv);
		auto parsedArgs = parser.ParseArguments();
		std::string yamlPath = std::filesystem::absolute(parsedArgs.YamlPath).string();

		IntermediateRepresentation irep{ YamlFrontendParser::ParseYamlFromFile(yamlPath) };
		auto [header, body] = CppGenerator::GenerateCppHeaderAndBody(irep);

		std::string outHeaderPath = std::filesystem::absolute(parsedArgs.OutDir).
			append(fmt::format("{0}.h", irep.GetClassName())).string();
		std::string outBodyPath = std::filesystem::absolute(parsedArgs.OutDir).
			append(fmt::format("{0}.cpp", irep.GetClassName())).string();
		WriteFile(outHeaderPath, header);
		WriteFile(outBodyPath, body);

		if (parsedArgs.Verbose)
		{
			PrintHeaderAndBody(irep.GetClassName(), header, body);
		}
	}
	catch (const YAML::Exception& e)
	{
		fmt::print("[!ERROR] Failed to Parse\n[!ERROR MSG] {0}\n", e.msg);
	}
	catch (const std::exception& e)
	{
		fmt::print("[!ERROR] Failed to Execute\n[!ERROR MSG] {0}\n", e.what());
	}
	return 0;
}

static void WriteFile(const std::string& path, const std::string& content) noexcept
{
	try
	{
		auto outStream = std::ofstream(path);
		outStream << content;
		outStream.close();
	}
	catch (const std::exception& e)
	{
		fmt::print("[!ERROR] Failed to write file in <{0}>\n[!ERROR MSG] {1}\n", path , e.what());
	}
}

static void PrintHeaderAndBody(const std::string className, const std::string& header, const std::string& body) noexcept
{
	fmt::print("// <==========| {0}.h |==========>\n\n", className);
	fmt::print("{0}\n\n", header);
	fmt::print("// <==========| {0}.cpp |==========>\n\n", className);
	fmt::print("{0}\n", body);
}

#ifdef DEBUG
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
		fmt::print("  Symbols: {0}\n", fmt::join(fg.GetSymbols(), " or "));
	}
}
#endif // DEBUG