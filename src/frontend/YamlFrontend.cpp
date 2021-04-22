#include "YamlFrontend.h"

constexpr auto DEFAULT_PROGRAMDESC = "Program";
constexpr auto DEFAULT_CLASSNAME = "ArgParser";

IntermediateRepresentation YamlFrontendParser::ParseYamlFromFile(const std::string filePath)
{
	IntermediateRepresentation irep;
	YAML::Node config = YAML::LoadFile(filePath);
	irep.SetProgramDescription(config[TAG_PROGDESC] ? config[TAG_PROGDESC].as<std::string>() : DEFAULT_PROGRAMDESC);
	irep.SetAngleIncludes(config[TAG_ANGLEINCLUDE] ? config[TAG_ANGLEINCLUDE].as<std::vector<std::string>>() : std::vector<std::string>{});
	irep.SetQuoteIncludes(config[TAG_QUOTEINCLUDE] ? config[TAG_QUOTEINCLUDE].as<std::vector<std::string>>() : std::vector<std::string>{});
	irep.SetNamespaceName(config[TAG_NAMESPACENAME] ? config[TAG_NAMESPACENAME].as<std::string>() : "");
	irep.SetClassName(config[TAG_CLASSNAME] ? config[TAG_CLASSNAME].as<std::string>() : DEFAULT_CLASSNAME);
	irep.SetPositionalArguments(config[TAG_POSTIONALARGS] ? config[TAG_POSTIONALARGS].as<std::vector<PositionalArgument>>() : std::vector<PositionalArgument>{});
	irep.SetOptionalArguments(config[TAG_OPTIONALARGS] ? config[TAG_OPTIONALARGS].as<std::vector<OptionalArgument>>() : std::vector<OptionalArgument>{});
	irep.SetFlags(config[TAG_FLAGS] ? config[TAG_FLAGS].as<std::vector<Flag>>() : std::vector<Flag>{});
	return irep;
}
