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

	inline DetourHook RunScriptThreads;
	bool RunScriptThreadsHook(pgPtrCollection* this_, uint32_t ops);

	//inline DetourHook RunScriptThreads2;
	//bool RunScriptThreadsHook2(uint32_t ops);

	// No Snipers: New Austin
	inline DetourHook ShootBullet;
	void ShootBulletHook(scrNativeCallContext* ctx);

	// No Snipers: Guarma
	inline DetourHook IsEntityInArea;
	BOOL IsEntityInAreaHook(scrNativeCallContext* ctx);

#if ENABLE_ANTI_ANTI_DEBUG
	inline DetourHook DebuggerCheck1;
	void DebuggerCheck1Hook(uint32_t a1);

	inline DetourHook DebuggerCheck2;
	void DebuggerCheck2Hook(int32_t a1, int32_t a2, int32_t a3);

	inline DetourHook IsDebuggerPresent;
	BOOL WINAPI IsDebuggerPresentHook();
#endif

	inline DetourHook DecreaseAmmo;
	void DecreaseAmmoHook(uint64_t this_, CPed* a2, uint64_t a3, uint32_t a4);

	inline DetourHook CreatePed;
	Ped CreatePedHook(scrNativeCallContext* ctx);

	inline DetourHook CreateVehicle;
	Vehicle CreateVehicleHook(scrNativeCallContext* ctx);

	inline DetourHook InventoryAddItem;
	BOOL InventoryAddItemHook(scrNativeCallContext* ctx);

	inline DetourHook GetGUIDFromItemID;
	BOOL GetGUIDFromItemIDHook(scrNativeCallContext* ctx);

	inline VMTHook SwapChain;
	inline constexpr size_t SwapChainMethodCount = 41, SwapChainPresentIndex = 8, SwapChainResizeBuffersIndex = 13;
	HRESULT STDMETHODCALLTYPE SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags);
	HRESULT STDMETHODCALLTYPE SwapChainResizeBuffersHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags);

	inline DetourHook CreatePersChar;
	PersChar CreatePersCharHook(scrNativeCallContext* ctx);

	inline DetourHook CreateAnimScene;
	AnimScene CreateAnimSceneHook(scrNativeCallContext* ctx);

	inline DetourHook DecorSetBool;
	BOOL DecorSetBoolHook(scrNativeCallContext* ctx);

	inline DetourHook DecorSetInt;
	BOOL DecorSetIntHook(scrNativeCallContext* ctx);

	inline DetourHook SetAnimSceneEntity;
	void SetAnimSceneEntityHook(scrNativeCallContext* ctx);

	inline DetourHook IsDlcPresent;
	BOOL IsDlcPresentHook(scrNativeCallContext* ctx);

	inline DetourHook GetAnimScenePed;
	Ped GetAnimScenePedHook(scrNativeCallContext* ctx);

	inline DetourHook CreateMetapedPed;
	Ped CreateMetapedPedHook(scrNativeCallContext* ctx);

	inline DetourHook CreateMetapedOutfitPed;
	Ped CreateMetapedOutfitPedHook(scrNativeCallContext* ctx);

	inline DetourHook ClonePed;
	Ped ClonePedHook(scrNativeCallContext* ctx);

	inline DetourHook ForceSpawnPersChar;
	Entity ForceSpawnPersCharHook(scrNativeCallContext* ctx);

	inline DetourHook ForceSpawnPersChar2;
	Entity ForceSpawnPersChar2Hook(scrNativeCallContext* ctx);

	inline DetourHook ForceSpawnPersChar3;
	void ForceSpawnPersChar3Hook(scrNativeCallContext* ctx);

	inline DetourHook ForceSpawnPersChar4;
	Any ForceSpawnPersChar4Hook(scrNativeCallContext* ctx);
}
