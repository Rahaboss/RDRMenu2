#include "pch.h"
#include "String.h"
#include "Container.h"

constexpr char Util::CharToUpper(char c)
{
	return (c >= 'a' && c <= 'z') ? c - ('a' - 'A') : c;
}

void Util::StringToUpper(std::string& String)
{
	std::transform(String.cbegin(), String.cend(), String.begin(), CharToUpper);
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

constexpr char Util::CharToLower(char c)
{
	return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

void Util::StringToLower(std::string& String)
{
	std::transform(String.cbegin(), String.cend(), String.begin(), CharToLower);
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

std::string Util::FormatString(_Printf_format_string_ const char* const Text, va_list Args)
{
	const int Format = _vscprintf(Text, Args);
	assert(Format >= 0);
	size_t Length = static_cast<size_t>(Format) + 1;
	char* Buffer = new char[Length * sizeof(char)];
	assert(Buffer);
	int Format2 = vsprintf_s(Buffer, Length, Text, Args);
	assert(Format2 >= 0);

	std::string Result{ Buffer };

	delete[] Buffer;

	return std::move(Result);
}

std::string Util::FormatString(_Printf_format_string_ const char* const Text, ...)
{
	va_list Args;
	va_start(Args, Text);

	std::string Result{ FormatString(Text, Args) };

	va_end(Args);

	return std::move(Result);
}
