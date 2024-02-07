#include "pch.h"
#include "Util/Console.h"
#include "Memory/Debugging.h"
#include "Config/Config.h"
#include "Config/Settings.h"
#include "Memory/Pointers.h"
#include "Thread/Fiber.h"
#include "Memory/Hooking.h"
#include "Renderer/Renderer.h"
#include "Thread/JobQueue.h"
#include "Script/Features.h"
#include "Util/Input.h"

void MainLoop()
{
	Console::Create();

	Debugging::Create();

	Config::Create();

	Settings::Create();

	Pointers::Create();

	Fiber MainFiber{ &Features::OnTick, &Fiber::MainFiberFunction };
	g_FiberCollection.push_back(&MainFiber);

	Hooking::Create();

	Renderer::Create();

	Hooking::Enable();

	// Main loop
	while (g_Running)
	{
		if (Util::IsKeyHeld(VK_LCONTROL) && Util::IsKeyHeld(VK_END))
			Features::StartExit();
		
		std::this_thread::sleep_for(10ms);
	}

	Hooking::Disable();
	
	Renderer::Destroy();
	
	Hooking::Destroy();

	Settings::Destroy();
	
	Debugging::Destroy();

	Console::Destroy();
}

BOOL WINAPI DllMain(HMODULE hModule, DWORD dwReason, LPVOID)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		// Disable DLL_THREAD_ATTACH and DLL_THREAD_DETACH
		DisableThreadLibraryCalls(hModule);

		// Set up global variables
		g_Module = hModule;
		g_GameModule = GetModuleHandle(NULL);
		g_BaseAddress = reinterpret_cast<uintptr_t>(g_GameModule);

		g_MainThread = CreateThread(NULL, 0, [](LPVOID) -> DWORD
			{
				// Start the main thread code
				MainLoop();

				// Close handle to main thread
				CloseHandle(g_MainThread);

				// Exit thread
				FreeLibraryAndExitThread(g_Module, 0);
			},
			NULL, 0, NULL);

		// Check if CreateThread failed
		if (!g_MainThread)
			return FALSE;
	}

	return TRUE;
}
