#include "pch.h"
#include "Console.h"

namespace Console
{
	static FILE* f;

	void Create()
	{
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"RDRMenu3 - "
	#ifdef _DEBUG
			"Debug"
	#else
			"Release"
	#endif
		);
	}

	void Destroy()
	{
		if (f)
			fclose(f);
		FreeConsole();
	}
}
