#include "OptionalArgument.h"

OptionalArgument::OptionalArgument()
	: PositionalArgument("", "", "", ""), m_Symbols{"-"}, m_DefaultValue("")
{
}

OptionalArgument::OptionalArgument(const std::string name, const std::string description, const std::string type, const std::string conversion, const std::vector<std::string> symbols, const std::string defaultValue)
	: PositionalArgument(name, description, type, conversion), m_Symbols(symbols), m_DefaultValue(defaultValue)
{
}

OptionalArgument::~OptionalArgument()
{
}

OptionalArgument::OptionalArgument(const OptionalArgument& other)
	: PositionalArgument(other), m_Symbols(other.m_Symbols), m_DefaultValue(other.m_DefaultValue)
{
}

OptionalArgument::OptionalArgument(OptionalArgument&& other) noexcept
	: PositionalArgument(std::move(other)), m_Symbols(std::move(other.m_Symbols)), m_DefaultValue(std::move(other.m_DefaultValue))
{
}

OptionalArgument& OptionalArgument::operator=(const OptionalArgument& other)
{
	if (this != &other)
	{
		PositionalArgument::operator=(other);
		m_Symbols = other.m_Symbols;
		m_DefaultValue = other.m_DefaultValue;
	}
	return *this;
}

OptionalArgument& OptionalArgument::operator=(OptionalArgument&& other) noexcept
{
	if (this != &other)
	{
		PositionalArgument::operator=(std::move(other));
		m_Symbols = std::move(other.m_Symbols);
		m_DefaultValue = std::move(other.m_DefaultValue);
	}
	return *this;
}
