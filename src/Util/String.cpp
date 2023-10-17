#include "pch.h"
#include "String.h"

void Util::StringToUpper(std::string& String)
{
	std::transform(String.cbegin(), String.cend(), String.begin(), ::toupper);
}

void Util::StringToLower(std::string& String)
{
	std::transform(String.cbegin(), String.cend(), String.begin(), ::tolower);
}

bool Util::IsStringValid(const char* String)
{
	return String && String[0];
}

bool Util::IsStringValid(const std::string& String)
{
	return !String.empty();
}
