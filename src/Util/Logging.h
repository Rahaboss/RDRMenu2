#pragma once

namespace Util
{
	void LogText(_Printf_format_string_ const char* const Text, ...);
	void LogClipboard(_Printf_format_string_ const char* const Text, ...);
}

#ifndef _DIST
// Simple printf
//#define LOG_TEXT(fmt, ...) printf_s(fmt ## "\n", __VA_ARGS__)

// Custom expandable log function
#define LOG_TEXT(...) ::Util::LogText(__VA_ARGS__)
#else
// Disabled logging in distribution build
#define LOG_TEXT(...) ((void)0)
#endif // !_DIST

#define LOG_CLIPBOARD(...) ::Util::LogClipboard(__VA_ARGS__)
