#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Renderer/Renderer.h"
#include "Script/Features.h"
#include "Thread/Fiber.h"
#include "Thread/Thread.h"
#include "Config/Settings.h"
#include "Rage/NativeInvoker.h"
#include "Script/PlayerInfo.h"

void Hooking::Create()
{
	LOG_TEXT("Creating hooks.");

	assert(MH_Initialize() == MH_OK);

	RunScriptThreads.Create(Pointers::RunScriptThreads, RunScriptThreadsHook);
	IsDLCPresent.Create(NativeInvoker::GetHandler(0x2763DC12BBE2BB6F), IsDLCPresentHook);
	DecreaseAmmo.Create(Pointers::DecreaseAmmo, DecreaseAmmoHook);
}

void Hooking::Destroy()
{
	LOG_TEXT("Destroying hooks.");
	
	DecreaseAmmo.Destroy();
	IsDLCPresent.Destroy();
	RunScriptThreads.Destroy();

	assert(MH_Uninitialize() == MH_OK);
}

void Hooking::Enable()
{
	LOG_TEXT("Enabling hooks.");

	assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);
}

void Hooking::Disable()
{
	LOG_TEXT("Disabling hooks.");

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

void Hooking::IsDLCPresentHook(rage::scrNativeCallContext* ctx)
{
	Hash dlcHash = ctx->GetArg<Hash>(0);

	IsDLCPresent.GetOriginal<decltype(&IsDLCPresentHook)>()(ctx);

	if (!g_Settings["enable_dlcs"].get<bool>())
		return;

	switch (dlcHash)
	{
	case RAGE_JOAAT("DLC_PREORDERCONTENT"):
	case RAGE_JOAAT("DLC_PHYSPREORDERCONTENT"):
	case RAGE_JOAAT("DLC_SPECIALEDITION"):
	case RAGE_JOAAT("DLC_ULTIMATEEDITION"):
	case RAGE_JOAAT("DLC_TREASUREMAP"):
		ctx->SetRet<BOOL>(TRUE);
		return;
	}

	LOG_TEXT("%s: Unknown DLC hash %u / 0x%X.", __FUNCTION__, dlcHash, dlcHash);
}

void Hooking::DecreaseAmmoHook(void* a1, rage::CPed* a2, uint64_t a3, uint32_t a4)
{
	if (a2 == g_LocalPlayer.m_Ped && g_Settings["weapon"]["infinite_ammo"].get<bool>())
		return;

	DecreaseAmmo.GetOriginal<decltype(&DecreaseAmmoHook)>()(a1, a2, a3, a4);
}
