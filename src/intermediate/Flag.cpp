#include "Flag.h"

Flag::Flag()
	: Argument("", "", ""), m_Symbols{ "-" }
{
}

Flag::Flag(const std::string name, const std::string description, const std::string type, const std::vector<std::string> symbols)
	: Argument(name, description, type), m_Symbols(symbols)
{
}

Flag::~Flag()
{
}

Flag::Flag(const Flag& other)
	: Argument(other), m_Symbols(other.m_Symbols)
{
}

Flag::Flag(Flag&& other) noexcept
	: Argument(std::move(other)), m_Symbols(std::move(other.m_Symbols))
{
}

Flag& Flag::operator=(const Flag& other)
{
	if (this != &other)
	{
		Argument::operator=(other);
		m_Symbols = other.m_Symbols;
	}
	return *this;
}

Flag& Flag::operator=(Flag&& other) noexcept
{
	if (this != &other)
	{
		Argument::operator=(std::move(other));
		m_Symbols = std::move(other.m_Symbols);
	}
	return *this;
}
