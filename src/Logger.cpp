#include "pch.h"
#include "Logger.h"
#include "Menu.h"

int LogText(const char* Format, ...)
{
	va_list Args;
	va_start(Args, Format);

	int Length = _vscprintf(Format, Args) + 1;
	char* Buffer = new char[Length * sizeof(char)];
	assert(Buffer);
	vsprintf_s(Buffer, Length, Format, Args);

	char tt[100]{};
	if (!_strtime_s(tt) && tt)
	{
		std::string str("[");
		str.append(tt);
		str.append("] ");
		str.append(Buffer);

		puts(str.c_str());
		LOG_TO_MENU(str.c_str());
	}

	delete[] Buffer;
	va_end(Args);
	return Length;
}
