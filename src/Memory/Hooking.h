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

#if ENABLE_VULKAN_RENDERER
	inline DetourHook vkQueuePresentKHR;
	VkResult vkQueuePresentKHRHook(VkQueue queue, const VkPresentInfoKHR* pPresentInfo);

	inline DetourHook vkQueueSubmit;
	VkResult vkQueueSubmitHook(VkQueue queue, uint32_t submitCount, const VkSubmitInfo* pSubmits, VkFence fence);
#endif // ENABLE_VULKAN_RENDERER

#if ENABLE_DEBUG_HOOKS
	inline DetourHook CreateAnimScene;
	void CreateAnimSceneHook(rage::scrNativeCallContext* ctx);

	inline DetourHook SetAnimSceneEntity;
	void SetAnimSceneEntityHook(rage::scrNativeCallContext* ctx);

	inline DetourHook StartAnimScene;
	void StartAnimSceneHook(rage::scrNativeCallContext* ctx);
	
	inline DetourHook SetAnimScenePlayList;
	void SetAnimScenePlayListHook(rage::scrNativeCallContext* ctx);
	
	inline DetourHook ApplyShopItemToPed;
	void ApplyShopItemToPedHook(rage::scrNativeCallContext* ctx);

	inline DetourHook ActivateInteriorSet;
	void ActivateInteriorSetHook(rage::scrNativeCallContext* ctx);

	inline DetourHook DeactivateInteriorSet;
	void DeactivateInteriorSetHook(rage::scrNativeCallContext* ctx);

	inline DetourHook SetBit;
	void SetBitHook(rage::scrNativeCallContext* ctx);

	inline DetourHook ClearBit;
	void ClearBitHook(rage::scrNativeCallContext* ctx);

	inline DetourHook RequestIPL;
	void RequestIPLHook(rage::scrNativeCallContext* ctx);

	inline DetourHook RemoveIPL;
	void RemoveIPLHook(rage::scrNativeCallContext* ctx);

	inline DetourHook VarString;
	void VarStringHook(rage::scrNativeCallContext* ctx);

	inline DetourHook PlayPedAmbientSpeech;
	void PlayPedAmbientSpeechHook(rage::scrNativeCallContext* ctx);

	inline DetourHook TaskStartScenarioInPlaceHash;
	void TaskStartScenarioInPlaceHashHook(rage::scrNativeCallContext* ctx);

	inline DetourHook TaskStartScenarioAtPosition;
	void TaskStartScenarioAtPositionHook(rage::scrNativeCallContext* ctx);

	inline DetourHook FindScenarioOfTypeHash;
	void FindScenarioOfTypeHashHook(rage::scrNativeCallContext* ctx);
#endif // ENABLE_DEBUG_HOOKS
}
