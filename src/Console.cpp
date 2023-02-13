#include "pch.h"
#include "Console.h"

namespace Console
{
	static FILE* s_InFile{};
	static FILE* s_OutFile{};
	static FILE* s_ErrFile{};

	void Create()
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
		LOG_TO_CONSOLE("Created console.\n");
	}

	void Destroy()
	{
		LOG_TO_CONSOLE("Destroying console.\n");
		fclose(s_ErrFile);
		fclose(s_OutFile);
		fclose(s_InFile);
		FreeConsole();
	}
}
