#pragma once

#include "pch.h"
#include "rage/scrThread.h"
#include "rage/scrNativeCallContext.h"
#include "rage/CPed.h"

typedef HRESULT(STDMETHODCALLTYPE* SwapChainPresent_t)(IDXGISwapChain3*, UINT, UINT);

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
	inline uint64_t* PedFactoryBase{};
	inline uint64_t* PedFactoryHash{};
	inline uint64_t* BlipBase{};
	inline uint64_t* BlipHash{};
	inline void* sub_7FF73D8C1638{};
	inline CPed* (*GetEntityPed)(Entity){};
	inline void* (*GetEntityAddress)(Entity){};
	inline uint32_t* dword_7FF74047DEB0{};
	inline uint64_t* qword_7FF74047DEB8{};
	inline int64_t** ScriptGlobals{};

	// D3D12 Renderer Stuff
	inline IDXGISwapChain3** SwapChain{};
	inline SwapChainPresent_t SwapChainPresent{};
	inline ID3D12CommandQueue** CommandQueue{};
}
