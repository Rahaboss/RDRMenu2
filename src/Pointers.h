#pragma once

#include "pch.h"
#include "Rage/scrThread.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/CPed.h"
#include "Rage/Obfuscation.h"
#include "Rage/CNetGamePlayer.h"
#include "Rage/CNetworkPlayerMgr.h"

typedef scrNativeHandler (*GetNativeHandler_t)(scrNativeHash);
typedef bool (*WorldToScreen_t)(const Vector3&, float&, float&);
typedef HRESULT (STDMETHODCALLTYPE *SwapChainPresent_t)(IDXGISwapChain3*, UINT, UINT);

namespace Pointers
{
	void Scan();

	inline bool* IsSessionStarted{};
	inline int* FrameCount{};
	inline pgPtrCollection* ThreadCollection{};
	inline void* RunScriptThreads{};
	inline void* RunScriptThreads2{};
	inline scrThread** ActiveThread{};
	inline GetNativeHandler_t GetNativeHandler{};
	inline CPed* (*GetPlayerPed)(Player){};
	inline void* DebuggerCheck1{};
	inline void* DebuggerCheck2{};
	inline void* DecreaseAmmo{};
	inline VariableEncryption* PedFactoryEncryption{};
	inline VariableEncryption* BlipEncryption{};
	inline CPed* (*GetEntityPed)(Entity){};
	inline void* (*GetEntityAddress)(Entity){};
	inline int64_t** ScriptGlobals{};
	inline WorldToScreen_t WorldToScreen{};
	inline void* (*sub_7FF73D8DB3C4)(void*){};
	inline CPed* (*GetEntityPedReal)(Entity){};
	inline uint32_t (*FwScriptGuidCreateGuid)(int64_t*){};
	inline VariableEncryption* PedPoolEncryption{};
	inline VariableEncryption* ObjectPoolEncryption{};
	inline VariableEncryption* VehiclePoolEncryption{};
	inline VariableEncryption* PickupPoolEncryption{};
	inline CNetGamePlayer* (*GetNetworkPlayer)(Player){};
	inline CNetworkPlayerMgr** NetworkPlayerMgr{};

	// D3D12 Renderer Stuff
	inline IDXGISwapChain3** SwapChain{};
	inline SwapChainPresent_t SwapChainPresent{};
	inline ID3D12CommandQueue** CommandQueue{};
}
