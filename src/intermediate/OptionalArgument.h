#pragma once

#include <vector>

#include "PositionalArgument.h"

class OptionalArgument : public PositionalArgument
{
private:
	std::vector<std::string> m_Symbols;
	std::string m_DefaultValue;

public:
	OptionalArgument();
	OptionalArgument(const std::string name, const std::string description, 
		const std::string type, const std::string conversion, const std::vector<std::string> symbols,
		const std::string defaultValue);
	~OptionalArgument();
	OptionalArgument(const OptionalArgument& other);
	OptionalArgument(OptionalArgument&& other) noexcept;
	OptionalArgument& operator=(const OptionalArgument& other);
	OptionalArgument& operator=(OptionalArgument&& other) noexcept;

	inline std::vector<std::string> GetSymbols() const { return m_Symbols; }
	inline void SetSymbols(const std::vector<std::string> symbols) { m_Symbols = symbols; }
	inline void SetSymbols(std::vector<std::string>&& symbols) { m_Symbols = std::move(symbols); }

	inline std::string GetDefaultValue() const { return m_DefaultValue; }
	inline void SetDefaultValue(const std::string value) { m_DefaultValue = value; }
};