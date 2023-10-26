#pragma once

#include "pch.h"

namespace Util
{
	void StringToUpper(std::string& String);
	std::string StringToUpperCopy(const std::string& String);
	std::string StringToUpperCopy(const char* String);
	void StringToLower(std::string& String);
	std::string StringToLowerCopy(const std::string& String);
	std::string StringToLowerCopy(const char* String);
	bool IsStringValid(const char* String);
	bool IsStringValid(const std::string& String);
	bool StringContains(const std::string& String, const std::string& Key);
}
