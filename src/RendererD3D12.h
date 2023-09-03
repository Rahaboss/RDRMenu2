#pragma once

#include "pch.h"

namespace RendererD3D12
{
	void Create();
	void Destroy();
	void Present(IDXGISwapChain3* SwapChain);
	void NewFrame();
	void EndFrame();

	struct _FrameContext
	{
		ID3D12CommandAllocator* CommandAllocator;
		ID3D12Resource* Resource;
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
	};

	inline IDXGISwapChain3* SwapChain;
	inline ID3D12CommandQueue* CommandQueue;
	inline ID3D12Device* Device;
	inline HWND Hwnd;
	inline UINT BuffersCounts;
	inline _FrameContext* FrameContext;
	inline ID3D12DescriptorHeap* DescriptorHeapImGuiRender;
	inline ID3D12GraphicsCommandList* CommandList;
	inline ID3D12DescriptorHeap* DescriptorHeapBackBuffers;
}
