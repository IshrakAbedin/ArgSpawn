#pragma once

#include <string>

class Argument
{
private:
	std::string m_Name;
	std::string m_Description;

public:
	Argument();
	Argument(const std::string name, const std::string description);
	virtual ~Argument();
	Argument(const Argument& other);
	Argument(Argument&& other) noexcept;
	Argument& operator=(const Argument& other);
	Argument& operator=(Argument&& other) noexcept;

	inline virtual std::string GetName() const { return m_Name; }
	inline virtual void SetName(const std::string name) { m_Name = name; }

	inline virtual std::string GetDescription() const { return m_Description; }
	inline virtual void SetDescription(const std::string description) { m_Description = description; }
};