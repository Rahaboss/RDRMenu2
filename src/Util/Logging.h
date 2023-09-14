#pragma once

void LogText(const char* const Text, ...);

#ifndef _DIST
// Simple printf
//#define LOG_TEXT(fmt, ...) printf_s(fmt ## "\n", __VA_ARGS__)

// Custom expandable log function
#define LOG_TEXT(...) LogText(__VA_ARGS__)
#else
// Disabled logging in distribution build
#define LOG_TEXT(...) ((void)0)
#endif // !_DIST
