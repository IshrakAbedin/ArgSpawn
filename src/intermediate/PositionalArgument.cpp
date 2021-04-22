#include "PositionalArgument.h"

PositionalArgument::PositionalArgument()
	: Argument("", ""), m_Type(""), m_Conversion("")
{
}

PositionalArgument::PositionalArgument(const std::string name, const std::string description, const std::string type, const std::string conversion)
	: Argument(name, description), m_Type(type), m_Conversion(conversion)
{
}

PositionalArgument::~PositionalArgument()
{
}

PositionalArgument::PositionalArgument(const PositionalArgument& other)
	: Argument(other), m_Type(other.m_Type), m_Conversion(other.m_Conversion)
{
}

PositionalArgument::PositionalArgument(PositionalArgument&& other) noexcept
	: Argument(std::move(other)), m_Type(std::move(other.m_Type)), m_Conversion(std::move(other.m_Conversion))
{
}

PositionalArgument& PositionalArgument::operator=(const PositionalArgument& other)
{
	if (this != &other)
	{
		Argument::operator=(other);
		m_Type = other.m_Type;
		m_Conversion = other.m_Conversion;
	}
	return *this;
}

PositionalArgument& PositionalArgument::operator=(PositionalArgument&& other) noexcept
{
	if (this != &other)
	{
		Argument::operator=(std::move(other));
		m_Type = std::move(other.m_Type);
		m_Conversion = std::move(other.m_Conversion);
	}
	return *this;
}
