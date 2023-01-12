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
		printf("Created console.\n");
	}

	void Destroy()
	{
		printf("Destroying console.\n");
		fclose(s_File);
		FreeConsole();
	}
}
