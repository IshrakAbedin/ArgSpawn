#include <string>
#include <vector>

#include "fmt/core.h"
#include "fmt/ranges.h"
#include "yaml-cpp/yaml.h"
#include "intermediate/IntermediateRepresentation.h"

#define YAML_SET(node, key, type, target)\
if (node[key]) target = node[key].as<type>()

struct Identity
{
    std::string Name{ "" };
    int Age{ 0 };
    std::vector<int> Numbers{};

    inline void PrintSelf()
    {
        fmt::print("Name : {0}\nAge : {1}\nNumbers : {2}\n", Name, Age, Numbers);
    }
};

namespace YAML {
    template<>
    struct convert<Identity> {
        static Node encode(const Identity& rhs) {
            Node node;
            node["name"] = rhs.Name;
            node["age"] = rhs.Age;
            node["numbers"] = rhs.Numbers;
            return node;
        }

        static bool decode(const Node& node, Identity& rhs) {
            if (!node.IsMap() || node.size() != 3) {
                return false;
            }

            rhs.Name = node["name"].as<std::string>();
            rhs.Age = node["age"].as<int>();
            rhs.Numbers = node["numbers"].as<std::vector<int>>();
            return true;
        }
    };
}

OptionalArgument GetOptArg()
{
    auto a = OptionalArgument("name", "desc", "int", "std::stoi", { "-n", "--name" }, "noname");
    return a;
}

int main()
{
    auto optArg = GetOptArg();
    std::vector<Identity> ids;
	YAML::Node config = YAML::LoadFile("./src/Test.yml");

	YAML_SET(config, "identities", std::vector<Identity>, ids);

    for (auto& id : ids)
    {
        id.PrintSelf();
    }

    fmt::print("------");
    fmt::print("Name: {0}\nDesc: {1}\nType: {2}\nConv: {3}\nSymbol: {4}\nDef: {5}\n---",
        optArg.GetName(), optArg.GetDescription(), optArg.GetType(),
        optArg.GetConversion(), fmt::join(optArg.GetSymbols(), " or "), optArg.GetDefaultValue());

	//fmt::print("Name : {0}\nAge : {1}\nNumbers : {2}\n", id.Name, id.Age, id.Numbers);
	return 0;
}