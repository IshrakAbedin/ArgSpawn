#include <string>
#include <vector>

#include "fmt/core.h"
#include "fmt/ranges.h"
#include "yaml-cpp/yaml.h"

#define YAML_SET(node, key, type, target)\
if (node[key]) target = node[key].as<type>()

struct Identity
{
	std::string Name;
	int Age;
	std::vector<int> Numbers;
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

int main()
{
	Identity id;
	YAML::Node config = YAML::LoadFile("./src/Test.yml");

	YAML_SET(config, "identity", Identity, id);

	fmt::print("Name : {0}\nAge : {1}\nNumbers : {2}\n", id.Name, id.Age, id.Numbers);
	return 0;
}