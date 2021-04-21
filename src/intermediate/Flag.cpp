#include "Flag.h"

Flag::Flag()
	:PositionalArgument("", "", "", ""), m_Symbols{ "-" }
{
}

Flag::Flag(const std::string name, const std::string description, const std::string type, const std::string conversion, const std::vector<std::string> symbols)
	: PositionalArgument(name, description, type, conversion), m_Symbols(symbols)
{
}

Flag::~Flag()
{
}

Flag::Flag(const Flag& other)
	: PositionalArgument(other), m_Symbols(other.m_Symbols)
{
}

Flag::Flag(Flag&& other) noexcept
	: PositionalArgument(std::move(other)), m_Symbols(std::move(other.m_Symbols))
{
}

Flag& Flag::operator=(const Flag& other)
{
	if (this != &other)
	{
		PositionalArgument::operator=(other);
		m_Symbols = other.m_Symbols;
	}
	return *this;
}

Flag& Flag::operator=(Flag&& other) noexcept
{
	if (this != &other)
	{
		PositionalArgument::operator=(std::move(other));
		m_Symbols = std::move(other.m_Symbols);
	}
	return *this;
}
