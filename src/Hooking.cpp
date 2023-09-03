#include "pch.h"
#include "Hooking.h"
#include "Signature.h"
#include "Renderer.h"

void Hooking::Create()
{
	std::cout << "Creating hooks.\n";

	assert(MH_Initialize() == MH_OK);
}

void Hooking::Destroy()
{
	std::cout << "Destroying hooks.\n";

	assert(MH_Uninitialize() == MH_OK);
}

static bool s_Enabled = false;
void Hooking::Enable()
{
	std::cout << "Enabling hooks.\n";

	if (s_Enabled)
		return;

	assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);

	s_Enabled = true;
}

void Hooking::Disable()
{
	std::cout << "Disabling hooks.\n";

	if (!s_Enabled)
		return;

	assert(MH_DisableHook(MH_ALL_HOOKS) == MH_OK);

	s_Enabled = false;
}

HRESULT STDMETHODCALLTYPE Hooking::SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags)
{
	if (g_Running)
		RendererD3D12::Present(SwapChain);

	return Hooking::SwapChain.GetOriginal<decltype(&SwapChainPresentHook)>(SwapChainPresentIndex)(SwapChain, SyncInterval, Flags);
}
