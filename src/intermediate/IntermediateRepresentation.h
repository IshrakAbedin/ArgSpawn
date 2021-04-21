#pragma once

#include "OptionalArgument.h"
#include "Flag.h"

class IntermediateRepresentation
{
private:
	std::string m_ProgramDescription;
	std::vector<std::string> m_AngleIncludes;
	std::vector<std::string> m_QuoteIncludes;
	std::string m_NamespaceName;
	std::string m_ClassName;
	std::vector<PositionalArgument> m_PositionalArguments;
	std::vector<OptionalArgument> m_OptionalArguments;
	std::vector<Flag> m_Flags;

public:
	IntermediateRepresentation();
	IntermediateRepresentation(const std::string programDescription, const std::vector<std::string> angleIncludes,
		const std::vector<std::string> quoteIncludes, const std::string namespaceName, const std::string className,
		const std::vector<PositionalArgument> postionalArguments, const std::vector<OptionalArgument> optionalArguments,
		const std::vector<Flag> flags);
	~IntermediateRepresentation();
	IntermediateRepresentation(const IntermediateRepresentation& other);
	IntermediateRepresentation(IntermediateRepresentation&& other) noexcept;
	IntermediateRepresentation& operator=(const IntermediateRepresentation& other);
	IntermediateRepresentation& operator=(IntermediateRepresentation&& other) noexcept;

	inline std::string GetProgramDescription() const { return m_ProgramDescription; }
	inline void SetProgramDescription(const std::string programDescription) { m_ProgramDescription = programDescription; }

	inline std::vector<std::string> GetAngleIncludes() const { return m_AngleIncludes; }
	inline std::vector<std::string>& GetAngleIncludesRef() { return m_AngleIncludes; }
	inline void SetAngleIncludes(const std::vector<std::string> angleIncludes) { m_AngleIncludes = angleIncludes; }
	inline void SetAngleIncludes(std::vector<std::string>&& angleIncludes) noexcept { m_AngleIncludes = std::move(angleIncludes); }

	inline std::vector<std::string> GetQuoteIncludes() const { return m_QuoteIncludes; }
	inline std::vector<std::string>& GetQuoteIncludesRef() { return m_QuoteIncludes; }
	inline void SetQuoteIncludes(const std::vector<std::string> quoteIncludes) { m_QuoteIncludes = quoteIncludes; }
	inline void SetQuoteIncludes(std::vector<std::string>&& quoteIncludes) noexcept { m_QuoteIncludes = std::move(quoteIncludes); }

	inline std::string GetNamespaceName() const { return m_NamespaceName; }
	inline void SetNamespaceName(const std::string namespaceName) { m_NamespaceName = namespaceName; }

	inline std::string GetClassName() const { return m_ClassName; }
	inline void SetClassName(const std::string className) { m_ClassName = className; }

	inline std::vector<PositionalArgument> GetPositionalArguments() const { return m_PositionalArguments; }
	inline std::vector<PositionalArgument>& GetPositionalArgumentsRef() { return m_PositionalArguments; }
	inline void SetPositionalArguments(const std::vector<PositionalArgument> positionalArguments) { m_PositionalArguments = positionalArguments; }
	inline void SetPositionalArguments(std::vector<PositionalArgument>&& positionalArguments) noexcept { m_PositionalArguments = std::move(positionalArguments); }

	inline std::vector<OptionalArgument> GetOptionalArguments() const { return m_OptionalArguments; }
	inline std::vector<OptionalArgument>& GetOptionalArgumentsRef() { return m_OptionalArguments; }
	inline void SetOptionalArguments(const std::vector<OptionalArgument> optionalArguments) { m_OptionalArguments = optionalArguments; }
	inline void SetOptionalArguments(std::vector<OptionalArgument>&& optionalArguments) noexcept { m_OptionalArguments = std::move(optionalArguments); }

	inline std::vector<Flag> GetFlags() const { return m_Flags; }
	inline std::vector<Flag>& GetFlagsRef() { return m_Flags; }
	inline void SetFlags(const std::vector<Flag> flags) { m_Flags = flags; }
	inline void SetFlags(std::vector<Flag>&& flags) noexcept { m_Flags = std::move(flags); }
};