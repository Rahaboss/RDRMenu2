#include "pch.h"
#include "Console.h"
#include "Pointers.h"
#include "Hooking.h"
#include "Renderer/Renderer.h"
#include "Fiber.h"
#include "Features.h"

void MainLoop()
{
	Console::Create();

	Pointers::Create();

	Fiber MainFiber{ &Features::OnTick };
	g_FiberCollection.push_back(&MainFiber);

	Hooking::Create();
	Hooking::Enable();

	Renderer::Create();

	// Main loop
	while (g_Running)
	{
		if (GetAsyncKeyState(VK_END))
			g_Running = false;

		std::this_thread::sleep_for(10ms);
	}
	
	Renderer::Destroy();

	Hooking::Disable();
	Hooking::Destroy();
	
	std::this_thread::sleep_for(3s);

	Console::Destroy();

	// Close handle to main thread
	CloseHandle(g_MainThread);

	// Exit thread
	FreeLibraryAndExitThread(g_Module, 0);
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
