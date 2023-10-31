#pragma once

#include "pch.h"

namespace Util
{
	void StringToUpper(std::string& String);
	std::string StringToUpperCopy(const std::string& String);
	std::string StringToUpperCopy(const char* String);
	template<typename size_t N>
	inline void StringToUpper(char(&String)[N])
	{
		for (size_t i = 0; i < N; i++)
			if (islower(static_cast<int>(String[i])))
				String[i] = static_cast<char>(toupper(static_cast<int>(String[i])));
	}
	void StringToLower(std::string& String);
	std::string StringToLowerCopy(const std::string& String);
	std::string StringToLowerCopy(const char* String);
	template<typename size_t N>
	inline void StringToLower(char (&String)[N])
	{
		for (size_t i = 0; i < N; i++)
			if (isupper(static_cast<int>(String[i])))
				String[i] = static_cast<char>(tolower(static_cast<int>(String[i])));
	}
	bool IsStringValid(const char* String);
	bool IsStringValid(const std::string& String);
	bool StringContains(const std::string& String, const std::string& Key);
}
