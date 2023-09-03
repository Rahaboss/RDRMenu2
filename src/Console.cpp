#include "pch.h"
#include "Console.h"

static FILE* s_InFile{};
static FILE* s_OutFile{};
static FILE* s_ErrFile{};

void Console::Create()
{
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

	std::cout << "Console created.\n";
}

void Console::Destroy()
{
	std::cout << "Destroying console.\n";

	fclose(s_ErrFile);
	fclose(s_OutFile);
	fclose(s_InFile);
	FreeConsole();
}
