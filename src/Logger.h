#pragma once

#include "pch.h"

void LogText(const char* Format, ...);
void LogToClipboard(const char* Format, ...);

#define LOG_TXT(...)			LogText(__VA_ARGS__)
#define LOG_TO_MENU(...)		Menu::Logger.AddLog(__VA_ARGS__)
#define LOG_TO_CONSOLE(...)		printf(__VA_ARGS__)
#define LOG_TO_CLIPBOARD(...)	LogToClipboard(__VA_ARGS__)
