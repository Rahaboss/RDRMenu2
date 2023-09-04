#include "pch.h"
#include "Console.h"
#include "Config/Config.h"
#include "Config/Settings.h"
#include "Memory/Pointers.h"
#include "Thread/Fiber.h"
#include "Memory/Hooking.h"
#include "Renderer/Renderer.h"
#include "Thread/JobQueue.h"
#include "Features.h"
#include "Input.h"

void MainLoop()
{
	Console::Create();

	Config::Create();

	Settings::Create();

	Pointers::Create();

	Fiber MainFiber{ &Features::OnTick };
	g_FiberCollection.push_back(&MainFiber);

	Fiber JobQueueFiber{ &JobQueue::Run };
	g_FiberCollection.push_back(&JobQueueFiber);

	Hooking::Create();
	Hooking::Enable();

	Renderer::Create();

	// Main loop
	while (g_Running)
	{
		if (Input::IsKeyHeld(VK_LCONTROL) && Input::IsKeyHeld(VK_END))
			Features::StartExit();

		std::this_thread::sleep_for(10ms);
	}
	
	Renderer::Destroy();

	Hooking::Disable();
	Hooking::Destroy();

	Settings::Destroy();

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
