#include "pch.h"
#include "String.h"
#include "Container.h"

void Util::StringToUpper(std::string& String)
{
	std::transform(String.cbegin(), String.cend(), String.begin(), ::toupper);
}

std::string Util::StringToUpperCopy(const std::string& String)
{
	std::string s = String;
	StringToUpper(s);
	return s;
}

std::string Util::StringToUpperCopy(const char* String)
{
	std::string s = String;
	StringToUpper(s);
	return s;
}

void Util::StringToLower(std::string& String)
{
	std::transform(String.cbegin(), String.cend(), String.begin(), ::tolower);
}

std::string Util::StringToLowerCopy(const std::string& String)
{
	std::string s = String;
	StringToLower(s);
	return s;
}

std::string Util::StringToLowerCopy(const char* String)
{
	std::string s = String;
	StringToLower(s);
	return s;
}

bool Util::IsStringValid(const char* String)
{
	return String && String[0];
}

bool Util::IsStringValid(const std::string& String)
{
	return !String.empty();
}

bool Util::StringContains(const std::string& String, const std::string& Key)
{
	return String.find(Key) != std::string::npos;
}
