#pragma once

#include "Argument.h"

class PositionalArgument : public Argument
{
private:
	std::string m_Conversion;

public:
	PositionalArgument();
	PositionalArgument(const std::string name, const std::string description,
		const std::string type, const std::string conversion);
	~PositionalArgument();
	PositionalArgument(const PositionalArgument& other);
	PositionalArgument(PositionalArgument&& other) noexcept;
	PositionalArgument& operator=(const PositionalArgument& other);
	PositionalArgument& operator=(PositionalArgument&& other) noexcept;

	inline std::string GetConversion() const { return m_Conversion; }
	inline void SetConversion(const std::string conversion) { m_Conversion = conversion; }
};