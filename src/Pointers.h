#pragma once

#include "pch.h"

typedef HRESULT(STDMETHODCALLTYPE* SwapChainPresent_t)(IDXGISwapChain1*, UINT, UINT);

namespace Pointers
{
	void Scan();

	inline bool* IsSessionStarted{};
	inline int* FrameCount{};
	inline IDXGISwapChain1** SwapChain{};
	inline SwapChainPresent_t SwapChainPresent{};
	inline ID3D12CommandQueue** CommandQueue{};
	inline void* ThreadCollection{};
	inline void* RunScriptThreads{};
	inline void* RunScriptThreads2{};
}
