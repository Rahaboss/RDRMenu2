#pragma once

#include "pch.h"

void LogText(const char* Format, ...);
void LogToClipboard(const char* Format, ...);

#define LOG_TXT(fmt, ...) LogText(fmt, __VA_ARGS__)
#define LOG_TO_MENU(fmt, ...) Menu::Logger.AddLog(fmt, __VA_ARGS__)
#define LOG_TO_CONSOLE(fmt, ...) printf(fmt, __VA_ARGS__)
#define LOG_TO_CLIPBOARD(fmt, ...) LogToClipboard(fmt, __VA_ARGS__)
