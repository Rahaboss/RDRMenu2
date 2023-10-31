#pragma once

#include "pch.h"
#include "DetourHook.h"
#include "VMTHook.h"
#include "Rage/scrThread.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/CPed.h"

namespace Hooking
{
	void Create();
	void Destroy();
	void Enable();
	void Disable();

	inline VMTHook SwapChain;
	inline constexpr size_t SwapChainMethodCount = 41, SwapChainPresentIndex = 8;
	HRESULT STDMETHODCALLTYPE SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags);

	inline DetourHook RunScriptThreads;
	bool RunScriptThreadsHook(rage::pgPtrCollection* this_, uint32_t ops);

	inline DetourHook IsDLCPresent;
	void IsDLCPresentHook(rage::scrNativeCallContext* ctx);

	inline DetourHook DecreaseAmmo;
	void DecreaseAmmoHook(void* a1, rage::CPed* a2, uint64_t a3, uint32_t a4);

	inline DetourHook ShootBullet;
	void ShootBulletHook(rage::scrNativeCallContext* ctx);

	inline DetourHook IsEntityInArea;
	void IsEntityInAreaHook(rage::scrNativeCallContext* ctx);

#ifndef _DIST
	inline DetourHook CreateAnimScene;
	void CreateAnimSceneHook(rage::scrNativeCallContext* ctx);

	inline DetourHook SetAnimSceneEntity;
	void SetAnimSceneEntityHook(rage::scrNativeCallContext* ctx);

	inline DetourHook StartAnimScene;
	void StartAnimSceneHook(rage::scrNativeCallContext* ctx);
	
	inline DetourHook SetAnimScenePlayList;
	void SetAnimScenePlayListHook(rage::scrNativeCallContext* ctx);
#endif // !_DIST
	
	inline DetourHook ApplyShopItemToPed;
	void ApplyShopItemToPedHook(rage::scrNativeCallContext* ctx);
}
