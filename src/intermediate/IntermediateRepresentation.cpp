#include "IntermediateRepresentation.h"

IntermediateRepresentation::IntermediateRepresentation()
	: m_ProgramDescription("Program"), m_NamespaceName(""), m_ClassName("ArgParser")
{
}

IntermediateRepresentation::IntermediateRepresentation(const std::string programDescription, const std::vector<std::string> angleIncludes, const std::vector<std::string> quoteIncludes, const std::string namespaceName, const std::string className, const std::vector<PositionalArgument> postionalArguments, const std::vector<OptionalArgument> optionalArguments, const std::vector<Flag> flags)
	: m_ProgramDescription(programDescription), m_AngleIncludes(m_AngleIncludes), m_QuoteIncludes(m_QuoteIncludes),
	m_NamespaceName(namespaceName), m_ClassName(className), m_PositionalArguments(postionalArguments),
	m_OptionalArguments(optionalArguments), m_Flags(flags)
{
}

IntermediateRepresentation::~IntermediateRepresentation()
{
}

IntermediateRepresentation::IntermediateRepresentation(const IntermediateRepresentation& other)
	: m_ProgramDescription(other.m_ProgramDescription), m_AngleIncludes(other.m_AngleIncludes),
	m_QuoteIncludes(other.m_QuoteIncludes), m_NamespaceName(other.m_NamespaceName), m_ClassName(other.m_ClassName),
	m_PositionalArguments(other.m_PositionalArguments), m_OptionalArguments(other.m_OptionalArguments),
	m_Flags(other.m_Flags)
{
}

IntermediateRepresentation::IntermediateRepresentation(IntermediateRepresentation&& other) noexcept
	: m_ProgramDescription(std::move(other.m_ProgramDescription)),
	m_AngleIncludes(std::move(other.m_AngleIncludes)),
	m_QuoteIncludes(std::move(other.m_QuoteIncludes)),
	m_NamespaceName(std::move(other.m_NamespaceName)),
	m_ClassName(std::move(other.m_ClassName)),
	m_PositionalArguments(std::move(other.m_PositionalArguments)),
	m_OptionalArguments(std::move(other.m_OptionalArguments)),
	m_Flags(std::move(other.m_Flags))
{
}

IntermediateRepresentation& IntermediateRepresentation::operator=(const IntermediateRepresentation& other)
{
	if (this != &other)
	{
		m_ProgramDescription = other.m_ProgramDescription;
		m_AngleIncludes = other.m_AngleIncludes;
		m_QuoteIncludes = other.m_QuoteIncludes;
		m_NamespaceName = other.m_NamespaceName;
		m_ClassName = other.m_ClassName;
		m_PositionalArguments = other.m_PositionalArguments;
		m_OptionalArguments = other.m_OptionalArguments;
		m_Flags = other.m_Flags;
	}
	return *this;
}

IntermediateRepresentation& IntermediateRepresentation::operator=(IntermediateRepresentation&& other) noexcept
{
	if (this != &other)
	{
		m_ProgramDescription = std::move(other.m_ProgramDescription);
		m_AngleIncludes = std::move(other.m_AngleIncludes);
		m_QuoteIncludes = std::move(other.m_QuoteIncludes);
		m_NamespaceName = std::move(other.m_NamespaceName);
		m_ClassName = std::move(other.m_ClassName);
		m_PositionalArguments = std::move(other.m_PositionalArguments);
		m_OptionalArguments = std::move(other.m_OptionalArguments);
		m_Flags = std::move(other.m_Flags);
	}
	return *this;
}
