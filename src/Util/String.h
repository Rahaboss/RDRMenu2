#pragma once

#include "pch.h"

namespace Util
{
	inline constexpr char CharToUpper(char c)
	{
		return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
	}
	template<typename size_t N>
	inline void StringToUpper(char(&String)[N])
	{
		for (size_t i = 0; i < N; i++)
			String[i] = Util::CharToUpper(String[i]);
	}
	void StringToUpper(std::string& String);
	std::string StringToUpperCopy(const std::string& String);
	std::string StringToUpperCopy(const char* String);
	
	inline constexpr char CharToLower(char c)
	{
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}
	template<typename size_t N>
	inline void StringToLower(char(&String)[N])
	{
		for (size_t i = 0; i < N; i++)
			String[i] = Util::CharToLower(String[i]);
	}
	void StringToLower(std::string& String);
	std::string StringToLowerCopy(const std::string& String);
	std::string StringToLowerCopy(const char* String);

	bool IsStringValid(const char* String);
	bool IsStringValid(const std::string& String);
	bool StringContains(const std::string& String, const std::string& Key);
}
