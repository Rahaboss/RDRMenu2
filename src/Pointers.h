#pragma once

#include "pch.h"

namespace Pointers
{
	void Create();

	// D3D12 renderer
	inline IDXGISwapChain3** SwapChain{};
	inline ID3D12CommandQueue** CommandQueue{};
}
