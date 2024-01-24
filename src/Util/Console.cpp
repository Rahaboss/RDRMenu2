#include "pch.h"
#include "Console.h"

#ifndef _DIST
static FILE* s_InFile{};
static FILE* s_OutFile{};
static FILE* s_ErrFile{};
static HANDLE s_hConsole{};
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

	s_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN);

	LOG_TEXT("Console created.");
#endif // !_DIST
}

void Console::Destroy()
{
#ifndef _DIST
	LOG_TEXT("Destroying console.");

	ResetColor();

	fclose(s_ErrFile);
	fclose(s_OutFile);
	fclose(s_InFile);
	FreeConsole();
#endif // !_DIST
}

void Console::SetColor(WORD Color)
{
	SetConsoleTextAttribute(s_hConsole, Color);
}

void Console::ResetColor()
{
	SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
