#include "pch.h"
#include "Logging.h"

void Util::LogText(_Printf_format_string_ const char* const Text, ...)
{
	va_list Args;
	va_start(Args, Text);

	int Format = _vscprintf(Text, Args);
	assert(Format >= 0);
	size_t Length = static_cast<size_t>(Format) + 1;
	char* Buffer = new char[Length * sizeof(char)];
	assert(Buffer);
	int Format2 = vsprintf_s(Buffer, Length, Text, Args);
	assert(Format2 >= 0);

	char tt[9]{};
	if (!_strtime_s(tt, 9) && tt[0])
	{
		std::string str;
		str.reserve(1 + 9 + 2 + Length + 1);
		str.append("[").append(tt).append("] ").append(Buffer);

		puts(str.c_str());
	}
	else
		puts(Buffer);

	delete[] Buffer;
	va_end(Args);
}
