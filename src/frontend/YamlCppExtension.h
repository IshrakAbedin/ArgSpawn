#pragma once

#include "yaml-cpp/yaml.h"

#include "intermediate/IntermediateRepresentation.h"

constexpr auto TAG_PROGDESC = "Program Description";
constexpr auto TAG_ANGLEINCLUDE = "Angle Includes";
constexpr auto TAG_QUOTEINCLUDE = "Quote Includes";
constexpr auto TAG_NAMESPACENAME = "Namespace Name";
constexpr auto TAG_CLASSNAME = "Class Name";
constexpr auto TAG_POSTIONALARGS = "Positional Arguments";
constexpr auto TAG_OPTIONALARGS = "Optional Arguments";
constexpr auto TAG_FLAGS = "Flags";
constexpr auto TAG_ARGNAME = "Name";
constexpr auto TAG_ARGDESC = "Description";
constexpr auto TAG_ARGTYPE = "Type";
constexpr auto TAG_ARGCONV = "Conversion";
constexpr auto TAG_ARGSYMBS = "Symbols";
constexpr auto TAG_ARGDEFAULTS = "Default Value";
constexpr auto COUNT_POSITIONALARGS = 4;
constexpr auto COUNT_POSITIONALARGSOPTIONAL = 1;
constexpr auto COUNT_OPTIONALARGS = 6;
constexpr auto COUNT_OPTIONALARGSOPTIONAL = 1;
constexpr auto COUNT_FLAGS = 5;
constexpr auto COUNT_FLAGSOPTIONAL = 2;

static inline bool ArgCountInRange(const long size, const long maxCount, const long optionalCount)
{
    return (size >= (maxCount - optionalCount) && size <= maxCount);
}

namespace YAML {
    template<>
    struct convert<PositionalArgument> {
        static Node encode(const PositionalArgument& rhs) {
            Node node;
            node[TAG_ARGNAME] = rhs.GetName();
            node[TAG_ARGDESC] = rhs.GetDescription();
            node[TAG_ARGTYPE] = rhs.GetType();
            node[TAG_ARGCONV] = rhs.GetConversion();
            return node;
        }

        static bool decode(const Node& node, PositionalArgument& rhs) {
            if (!node.IsMap() || !ArgCountInRange(node.size(), COUNT_POSITIONALARGS, COUNT_POSITIONALARGSOPTIONAL)) {
                return false;
            }

            rhs.SetName(node[TAG_ARGNAME].as<std::string>());
            rhs.SetDescription(node[TAG_ARGDESC].as<std::string>());
            rhs.SetType(node[TAG_ARGTYPE].as<std::string>());
            auto a = node[TAG_ARGCONV];
            rhs.SetConversion(node[TAG_ARGCONV] ? node[TAG_ARGCONV].as<std::string>() : "");
            return true;
        }
    };

    template<>
    struct convert<OptionalArgument> {
        static Node encode(const OptionalArgument& rhs) {
            Node node;
            node[TAG_ARGNAME] = rhs.GetName();
            node[TAG_ARGDESC] = rhs.GetDescription();
            node[TAG_ARGTYPE] = rhs.GetType();
            node[TAG_ARGCONV] = rhs.GetConversion();
            node[TAG_ARGSYMBS] = rhs.GetSymbols();
            node[TAG_ARGDEFAULTS] = rhs.GetDefaultValue();
            return node;
        }

        static bool decode(const Node& node, OptionalArgument& rhs) {
            if (!node.IsMap() || !ArgCountInRange(node.size(), COUNT_OPTIONALARGS, COUNT_OPTIONALARGSOPTIONAL)) {
                return false;
            }

            rhs.SetName(node[TAG_ARGNAME].as<std::string>());
            rhs.SetDescription(node[TAG_ARGDESC].as<std::string>());
            rhs.SetType(node[TAG_ARGTYPE].as<std::string>());
            rhs.SetConversion(node[TAG_ARGCONV] ? node[TAG_ARGCONV].as<std::string>() : "");
            rhs.SetSymbols(node[TAG_ARGSYMBS].as<std::vector<std::string>>());
            rhs.SetDefaultValue(node[TAG_ARGDEFAULTS].as<std::string>());
            return true;
        }
    };

    template<>
    struct convert<Flag> {
        static Node encode(const Flag& rhs) {
            Node node;
            node[TAG_ARGNAME] = rhs.GetName();
            node[TAG_ARGDESC] = rhs.GetDescription();
            node[TAG_ARGTYPE] = rhs.GetType();
            node[TAG_ARGCONV] = rhs.GetConversion();
            node[TAG_ARGSYMBS] = rhs.GetSymbols();
            return node;
        }

        static bool decode(const Node& node, Flag& rhs) {
            if (!node.IsMap() || !ArgCountInRange(node.size(), COUNT_FLAGS, COUNT_FLAGSOPTIONAL)) {
                return false;
            }

            rhs.SetName(node[TAG_ARGNAME].as<std::string>());
            rhs.SetDescription(node[TAG_ARGDESC].as<std::string>());
            rhs.SetType(node[TAG_ARGTYPE] ? node[TAG_ARGTYPE].as<std::string>() : "bool");
            rhs.SetConversion(node[TAG_ARGCONV] ? node[TAG_ARGCONV].as<std::string>() : "(bool)std::stoi");
            rhs.SetSymbols(node[TAG_ARGSYMBS].as<std::vector<std::string>>());

            return true;
        }
    };
}