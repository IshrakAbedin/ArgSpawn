#include "Argument.h"

Argument::Argument()
	: Argument::Argument("", "", "")
{
}

Argument::Argument(const std::string name, const std::string description, const std::string type)
	: m_Name(name), m_Description(description), m_Type(type)
{
}

Argument::~Argument()
{
}

Argument::Argument(const Argument& other)
	:m_Name(other.m_Name), m_Description(other.m_Description), m_Type(other.m_Type)
{
}

Argument::Argument(Argument&& other) noexcept
	: m_Name(std::move(other.m_Name)), m_Description(std::move(other.m_Description)), m_Type(std::move(other.m_Type))
{
}

Argument& Argument::operator=(const Argument& other)
{
    if (this != &other)
    {
		m_Name = other.m_Name;
		m_Description = other.m_Description;
		m_Type = other.m_Type;
    }
    return *this;
}

Argument& Argument::operator=(Argument&& other) noexcept
{
	if (this != &other)
	{
		m_Name = std::move(other.m_Name);
		m_Description = std::move(other.m_Description);
		m_Type = std::move(other.m_Type);
	}
	return *this;
}
