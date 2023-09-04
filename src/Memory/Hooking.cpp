#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Renderer/Renderer.h"
#include "Features.h"
#include "Thread/Fiber.h"
#include "Thread/Thread.h"

void Hooking::Create()
{
	std::cout << "Creating hooks.\n";

	assert(MH_Initialize() == MH_OK);

	RunScriptThreads.Create(Pointers::RunScriptThreads, RunScriptThreadsHook);
}

void Hooking::Destroy()
{
	std::cout << "Destroying hooks.\n";
	
	RunScriptThreads.Destroy();

	assert(MH_Uninitialize() == MH_OK);
}

void Hooking::Enable()
{
	std::cout << "Enabling hooks.\n";

	assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);
}

void Hooking::Disable()
{
	std::cout << "Disabling hooks.\n";

	assert(MH_DisableHook(MH_ALL_HOOKS) == MH_OK);
}

HRESULT STDMETHODCALLTYPE Hooking::SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags)
{
	if (g_Running)
		RendererD3D12::Present(SwapChain);

	return Hooking::SwapChain.GetOriginal<decltype(&SwapChainPresentHook)>(SwapChainPresentIndex)(SwapChain, SyncInterval, Flags);
}

bool Hooking::RunScriptThreadsHook(rage::pgPtrCollection* this_, uint32_t ops)
{
	const bool Result = RunScriptThreads.GetOriginal<decltype(&RunScriptThreadsHook)>()(this_, ops);

	if (g_Running)
		Thread::ExecuteAsThread(RAGE_JOAAT("main"), &Fiber::ScriptThreadTick);

	return Result;
}
