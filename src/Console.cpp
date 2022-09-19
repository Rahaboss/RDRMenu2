#include "pch.h"
#include "Console.h"

namespace Console
{
	static FILE* f;

	void Create()
	{
		AllocConsole();
		freopen_s(&f, "CONOUT$", "w", stdout);
		SetConsoleTitle(L"RDRMenu2 - "
	#ifdef _DEBUG
			"Debug"
	#else
			"Release"
	#endif
		);
		std::cout << "Created console.\n";
	}

	void Destroy()
	{
		std::cout << "Destroying console.\n";
		std::this_thread::sleep_for(500ms);
		if (f)
			fclose(f);
		FreeConsole();
	}
}
