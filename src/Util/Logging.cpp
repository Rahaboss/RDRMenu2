#include "pch.h"
#include "Logging.h"
#include "String.h"

void Util::LogText(_Printf_format_string_ const char* const Text, ...)
{
	va_list Args;
	va_start(Args, Text);

	std::string String = FormatString(Text, Args);

	// Add timestamp [HH:MM:SS] before formatted text
	char tt[9]{};
	if (!_strtime_s(tt, 9) && tt[0])
	{
		std::string s2;
		s2.reserve(12);
		s2.append("[").append(tt).append("] ");
		String = s2 + String;
	}
	
	puts(String.c_str());

	va_end(Args);
}

void Util::LogClipboard(_Printf_format_string_ const char* const Text, ...)
{
	va_list Args;
	va_start(Args, Text);

	std::string String = FormatString(Text, Args);

	ImGui::SetClipboardText(String.c_str()); // cba

	va_end(Args);
}
