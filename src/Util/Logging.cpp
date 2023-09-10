#include "pch.h"
#include "Logging.h"

void LogText(const char* const Text, ...)
{
	va_list Args;
	va_start(Args, Text);

	size_t Length = static_cast<size_t>(_vscprintf(Text, Args)) + 1;
	char* Buffer = new char[Length * sizeof(char)];
	assert(Buffer);
	vsprintf_s(Buffer, Length, Text, Args);

	char tt[9]{};
	if (!_strtime_s(tt) && tt[0] && false)
	{
		std::string str("[");
		str.append(tt);
		str.append("] ");
		str.append(Buffer);

		puts(str.c_str());
	}
	else
		puts(Buffer);

	delete[] Buffer;
	va_end(Args);
}
