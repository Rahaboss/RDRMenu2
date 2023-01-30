#include "pch.h"
#include "Console.h"

namespace Console
{
	static FILE* s_File{};

	void Create()
	{
		AllocConsole();
		freopen_s(&s_File, "CONOUT$", "w", stdout);
		assert(s_File);
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
		fclose(s_File);
		FreeConsole();
	}
}
