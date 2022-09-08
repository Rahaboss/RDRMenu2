#pragma once

#include "pch.h"

typedef HRESULT(STDMETHODCALLTYPE* SwapChainPresent_t)(UINT, UINT);

namespace Pointers
{
	void Scan();

	inline int* FrameCount{};
	inline IDXGISwapChain1** SwapChain{};
	inline SwapChainPresent_t SwapChainPresent{};
	inline ID3D12CommandQueue** CommandQueue{};
}
