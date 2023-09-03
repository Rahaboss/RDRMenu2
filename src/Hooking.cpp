#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Renderer/Renderer.h"
#include "Features.h"
#include "Fiber.h"

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

static bool s_Enabled = false;
void Hooking::Enable()
{
	if (s_Enabled)
		return;

	std::cout << "Enabling hooks.\n";

	assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);
	s_Enabled = true;
}

void Hooking::Disable()
{
	if (!s_Enabled)
		return;

	std::cout << "Disabling hooks.\n";

	assert(MH_DisableHook(MH_ALL_HOOKS) == MH_OK);
	s_Enabled = false;
}

HRESULT STDMETHODCALLTYPE Hooking::SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags)
{
	if (g_Running)
		RendererD3D12::Present(SwapChain);

	return Hooking::SwapChain.GetOriginal<decltype(&SwapChainPresentHook)>(SwapChainPresentIndex)(SwapChain, SyncInterval, Flags);
}

bool Hooking::RunScriptThreadsHook(rage::pgPtrCollection* this_, uint32_t ops)
{
	bool Result = RunScriptThreads.GetOriginal<decltype(&RunScriptThreadsHook)>()(this_, ops);

	if (g_Running)
		Features::ExecuteAsThread(RAGE_JOAAT("main"), &Fiber::ScriptThreadTick);

	return Result;
}
