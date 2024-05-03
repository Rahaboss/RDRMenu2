#pragma once

#include "pch.h"

namespace Util
{
	constexpr char CharToUpper(char c);
	void StringToUpper(std::string& String);
	std::string StringToUpperCopy(const std::string& String);
	std::string StringToUpperCopy(const char* String);
	constexpr char CharToLower(char c);
	void StringToLower(std::string& String);
	std::string StringToLowerCopy(const std::string& String);
	std::string StringToLowerCopy(const char* String);
	bool IsStringValid(const char* String);
	bool IsStringValid(const std::string& String);
	bool StringContains(const std::string& String, const std::string& Key);
	std::string FormatString(_Printf_format_string_ const char* const Text, va_list Args);
	std::string FormatString(_Printf_format_string_ const char* const Text, ...);
	
	template<typename size_t N>
	inline constexpr void StringToUpper(char (&String)[N])
	{
		for (size_t i = 0; i < N; i++)
			String[i] = Util::CharToUpper(String[i]);
	}

	template<typename size_t N>
	inline constexpr void StringToLower(char (&String)[N])
	{
		for (size_t i = 0; i < N; i++)
			String[i] = Util::CharToLower(String[i]);
	}
}
