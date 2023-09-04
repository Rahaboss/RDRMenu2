#pragma once

#include "pch.h"
#include "Rage/scrThread.h"
#include "Rage/scrNativeCallContext.h"

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
}
