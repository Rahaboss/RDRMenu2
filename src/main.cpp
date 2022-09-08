#include "pch.h"
#include "Console.h"

void MainLoop()
{
	TRY
	{
		Console::Create();

		std::cout << "Base address: " << LOG_HEX(g_base_address) << "\n";
		std::cout << "Press END to eject.\n";

		while (g_running)
		{
			if (GetAsyncKeyState(VK_END))
				g_running = false;
			std::this_thread::sleep_for(25ms);
		}

		Console::Destroy();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

BOOL WINAPI DllMain(HMODULE hmodule, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmodule);
		
		g_hmodule = hmodule;
		g_game_module = GetModuleHandle(NULL);
		g_base_address = reinterpret_cast<uintptr_t>(g_game_module);
		g_main_thread = CreateThread(NULL, 0, [](LPVOID) -> DWORD
			{
				MainLoop();
				CloseHandle(g_main_thread);
				FreeLibraryAndExitThread(g_hmodule, 0);
			},
			NULL, 0, NULL);

		if (!g_main_thread)
			return FALSE;
	}

	return TRUE;
}
