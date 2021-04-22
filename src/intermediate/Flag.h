#pragma once

#include <vector>

#include "PositionalArgument.h"

class Flag : public PositionalArgument
{
private:
	std::vector<std::string> m_Symbols;

public:
	Flag();
	Flag(const std::string name, const std::string description, 
		const std::string type, const std::string conversion, const std::vector<std::string> symbols);
	~Flag();
	Flag(const Flag& other);
	Flag(Flag&& other) noexcept;
	Flag& operator=(const Flag& other);
	Flag& operator=(Flag&& other) noexcept;

	inline std::vector<std::string> GetSymbols() const { return m_Symbols; }
	//inline void SetSymbols(const std::vector<std::string> symbols) { m_Symbols = symbols; }
	inline void SetSymbols(std::vector<std::string>&& symbols) { m_Symbols = std::move(symbols); }
};