#include "pch.h"
#include "Console.h"
#include "Pointers.h"
#include "Hooking.h"

void MainLoop()
{
	TRY
	{
		Console::Create();

		std::cout << "Scanning for pointers.\n";
		Pointers::Scan();

		std::cout << "Creating hooks.\n";
		Hooking::Create();

		std::cout << "Enabling hooks.\n";
		Hooking::Enable();

		std::cout << "Thread Collection: " << LOG_HEX(Pointers::ThreadCollection) << "\n";

		while (g_running)
		{
			if (GetAsyncKeyState(VK_END))
				g_running = false;
			std::this_thread::sleep_for(25ms);
		}

		std::cout << "Disabling hooks.\n";
		Hooking::Disable();

		std::cout << "Destroying hooks.\n";
		Hooking::Destroy();

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
