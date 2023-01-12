#include "pch.h"
#include "Console.h"
#include "Pointers.h"
#include "Features.h"
#include "Fiber.h"
#include "Hooking.h"
#include "Renderer.h"

void MainLoop()
{
	TRY
	{
		Console::Create();

		Settings::Create();

		Pointers::Scan();

		Fiber MainFiber(Features::OnTick);
		g_FiberCollection.push_back(&MainFiber);

		Fiber JobQueueFiber(Features::RunJobQueue);
		g_FiberCollection.push_back(&JobQueueFiber);

		Hooking::Create();

		Renderer::Create();
		
		Hooking::Enable();

		while (g_Running)
		{
			if (Features::IsKeyHeld(VK_LCONTROL) && Features::IsKeyHeld(VK_END))
				g_Running = false;
			std::this_thread::sleep_for(25ms);
		}

		Hooking::Disable();

		Renderer::Destroy();
		
		Hooking::Destroy();

		JobQueueFiber.Destroy();

		MainFiber.Destroy();

		Settings::Destroy();

		Console::Destroy();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

BOOL WINAPI DllMain(HMODULE hmodule, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hmodule);
		
		g_Module = hmodule;
		g_GameModule = GetModuleHandle(NULL);
		g_BaseAddress = reinterpret_cast<uintptr_t>(g_GameModule);
		g_MainThread = CreateThread(NULL, 0, [](LPVOID) -> DWORD
			{
				MainLoop();
				CloseHandle(g_MainThread);
				FreeLibraryAndExitThread(g_Module, 0);
			},
			NULL, 0, NULL);

		if (!g_MainThread)
			return FALSE;
	}

	return TRUE;
}
