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
