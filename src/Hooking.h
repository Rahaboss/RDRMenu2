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

	inline size_t HookCount = 0;

	inline DetourHook RunScriptThreads;
	bool RunScriptThreadsHook(rage::pgPtrCollection* this_, uint32_t ops);

	inline DetourHook RunScriptThreads2;
	bool RunScriptThreadsHook2(uint32_t ops);

	// No Snipers: New Austin
	inline DetourHook ShootBullet;
	void ShootBulletHook(rage::scrNativeCallContext* ctx);

	// No Snipers: Guarma
	inline DetourHook IsEntityInArea;
	BOOL IsEntityInAreaHook(rage::scrNativeCallContext* ctx);

#if ENABLE_ANTI_ANTI_DEBUG
	inline DetourHook DebuggerCheck1;
	void DebuggerCheck1Hook(uint32_t a1);

	inline DetourHook DebuggerCheck2;
	void DebuggerCheck2Hook(int32_t a1, int32_t a2, int32_t a3);

	inline DetourHook IsDebuggerPresent;
	BOOL WINAPI IsDebuggerPresentHook();
#endif

	inline DetourHook DecreaseAmmo;
	void DecreaseAmmoHook(uint64_t this_, rage::CPed* a2, uint64_t a3, uint32_t a4);

	inline DetourHook CreatePed;
	Ped CreatePedHook(rage::scrNativeCallContext* ctx);

	inline DetourHook CreateVehicle;
	Vehicle CreateVehicleHook(rage::scrNativeCallContext* ctx);

	inline DetourHook InventoryAddItem;
	BOOL InventoryAddItemHook(rage::scrNativeCallContext* ctx);

	inline DetourHook GetGUIDFromItemID;
	BOOL GetGUIDFromItemIDHook(rage::scrNativeCallContext* ctx);

	inline VMTHook SwapChain;
	inline constexpr size_t SwapChainMethodCount = 41, SwapChainPresentIndex = 8, SwapChainResizeBuffersIndex = 13;
	HRESULT STDMETHODCALLTYPE SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags);
	HRESULT STDMETHODCALLTYPE SwapChainResizeBuffersHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

	inline DetourHook CreatePersChar;
	PersChar CreatePersCharHook(rage::scrNativeCallContext* ctx);

	inline DetourHook CreateAnimScene;
	AnimScene CreateAnimSceneHook(rage::scrNativeCallContext* ctx);

	inline DetourHook DecorSetBool;
	BOOL DecorSetBoolHook(rage::scrNativeCallContext* ctx);

	inline DetourHook DecorSetInt;
	BOOL DecorSetIntHook(rage::scrNativeCallContext* ctx);

	inline DetourHook SetAnimSceneEntity;
	void SetAnimSceneEntityHook(rage::scrNativeCallContext* ctx);

	inline DetourHook IsDlcPresent;
	BOOL IsDlcPresentHook(rage::scrNativeCallContext* ctx);

	inline DetourHook CreateObject;
	Object CreateObjectHook(rage::scrNativeCallContext* ctx);

	inline DetourHook CreateObjectNoOffset;
	Object CreateObjectNoOffsetHook(rage::scrNativeCallContext* ctx);

	inline DetourHook GetLabelText2;
	const char* GetLabelText2Hook(rage::scrNativeCallContext* ctx);

	inline DetourHook GetCharFromAudioConvFilename;
	const char* GetCharFromAudioConvFilenameHook(rage::scrNativeCallContext* ctx);
}
