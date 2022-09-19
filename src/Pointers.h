#pragma once

#include "pch.h"
#include "rage/scrThread.h"
#include "rage/scrNativeCallContext.h"

typedef HRESULT(STDMETHODCALLTYPE* SwapChainPresent_t)(IDXGISwapChain1*, UINT, UINT);

namespace Pointers
{
	void Scan();

	inline bool* IsSessionStarted{};
	inline int* FrameCount{};
	inline IDXGISwapChain1** SwapChain{};
	inline SwapChainPresent_t SwapChainPresent{};
	inline ID3D12CommandQueue** CommandQueue{};
	inline pgPtrCollection* ThreadCollection{};
	inline void* RunScriptThreads{};
	inline void* RunScriptThreads2{};
	inline scrThread** ActiveThread{};
	inline GetNativeHandler_t GetNativeHandler{};
	inline class CPed* (*GetPlayerPed)(Player){};
	inline void* DebuggerCheck1{};
	inline void* DebuggerCheck2{};
}
