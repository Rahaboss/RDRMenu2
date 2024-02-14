#pragma once

#include "pch.h"
#include "Rage/scrThread.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/Obfuscation.h"
#include "Rage/CPed.h"
#include "Rage/enums.h"
#include "Rage/CNetworkPlayerMgr.h"

namespace Pointers
{
	void Create();

	// D3D12 renderer
	inline IDXGISwapChain3** SwapChain{};
	inline ID3D12CommandQueue** CommandQueue{};

	// RAGE
	inline rage::pgPtrCollection* ThreadCollection{};
	inline void* RunScriptThreads{};
	inline rage::scrThread** ActiveThread{};
	inline rage::scrNativeHandler (*GetNativeHandler)(rage::scrNativeHash){};
	inline int64_t** ScriptGlobals{};
	inline rage::CPed* (*GetPlayerPed)(Player){};
	inline VariableEncryption* PedFactoryEncryption{};
	inline void (*DecreaseAmmo)(void*, rage::CPed*, uint64_t, uint32_t){};
	// GRAPHICS::GET_SCREEN_COORD_FROM_WORLD_COORD
	inline bool (*WorldToScreen)(const Vector3&, float&, float&){};
	// PED::GET_PED_BONE_COORDS
	inline Vector3* (*GetPedBoneCoords)(Vector3&, Ped, int, const Vector3&){};
	inline rage::CPed* (*GetPedAddress)(Ped){};
	// PED::GET_PED_BONE_INDEX
	inline int (*GetPedBoneIndex)(rage::CPed*, int){};
	inline uint32_t (*fwScriptGuidCreateGuid)(void*){};
	inline VariableEncryption* PedPoolEncryption{};
	inline VariableEncryption* ObjectPoolEncryption{};
	inline VariableEncryption* VehiclePoolEncryption{};
	inline VariableEncryption* PickupPoolEncryption{};
	// ENTITY::DOES_ENTITY_EXIST
	inline bool (*DoesEntityExist)(Entity){};
	// ENTITY::GET_ENTITY_MODEL
	inline Hash (*GetEntityModel)(Entity){};
	// ENTITY::GET_ENTITY_COORDS
	inline Vector3* (*GetEntityCoords)(Vector3&, Entity, bool, bool){};
	inline const char* (*GetStringFromHashKey)(Hash){};
	inline void* ReturnAddressSpoof{};
	// PED::_GET_META_PED_TYPE
	inline eMetaPedType (*GetMetaPedType)(Ped){};
	// ENTITY::GET_ENTITY_HEALTH
	inline int (*GetEntityHealth)(Entity){};
	inline void* CheckForDebugger{}; // cmp [rbx+2], al
	inline void* TerminateProcess{};
	inline bool* IsSessionStarted{};
	inline rage::CNetworkPlayerMgr** NetworkPlayerMgr{};
	inline rage::netPlayerData* (*GetOfflineNetPlayerData)(){};

#if ENABLE_VULKAN_RENDERER
	// Vulkan renderer
	inline VkInstance* vkInstance{};
	inline VkPhysicalDevice* vkPhysicalDevice{};
	inline VkDevice* vkDevice{};
	inline VkQueue* vkQueue{};
	inline PFN_vkVoidFunction (*vkGetDeviceProcAddr)(VkDevice, const char*){};
	inline VkResult (*vkQueuePresentKHR)(VkQueue, const VkPresentInfoKHR*){};
	inline VkResult (*vkQueueSubmit)(VkQueue, uint32_t, const VkSubmitInfo*, VkFence){};
#endif // ENABLE_VULKAN_RENDERER
}
