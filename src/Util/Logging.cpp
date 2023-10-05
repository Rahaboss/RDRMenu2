#include "pch.h"
#include "Logging.h"

void Util::LogText(const char* const Text, ...)
{
	va_list Args;
	va_start(Args, Text);

	size_t Length = static_cast<size_t>(_vscprintf(Text, Args)) + 1;
	char* Buffer = new char[Length * sizeof(char)];
	assert(Buffer);
	vsprintf_s(Buffer, Length, Text, Args);

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
