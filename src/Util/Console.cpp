#include "pch.h"
#include "Console.h"

#ifndef _DIST
static FILE* s_InFile{};
static FILE* s_OutFile{};
static FILE* s_ErrFile{};
#endif // !_DIST

void Console::Create()
{
#ifndef _DIST
	AllocConsole();
	freopen_s(&s_InFile, "CONIN$", "r", stdin);
	freopen_s(&s_OutFile, "CONOUT$", "w", stdout);
	freopen_s(&s_ErrFile, "CONOUT$", "w", stderr);
	assert(s_InFile && s_OutFile && s_ErrFile);
	SetConsoleTitle(L"RDRMenu2 - "
#ifdef _DEBUG
		"Debug"
#else
		"Release"
#endif
	);

	LOG_TEXT("Console created.\n");
#endif // !_DIST
}

void Console::Destroy()
{
#ifndef _DIST
	LOG_TEXT("Destroying console.\n");

	fclose(s_ErrFile);
	fclose(s_OutFile);
	fclose(s_InFile);
	FreeConsole();
#endif // !_DIST
}
