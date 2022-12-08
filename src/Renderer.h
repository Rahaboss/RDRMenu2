#pragma once

#include "pch.h"

namespace Renderer
{
	void Create();
	void Destroy();
	void Present();
	void NewFrame();
	void EndFrame();
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	// Must be false on inject, otherwise breaks input
	inline bool MenuOpen = false;

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
	inline LONG_PTR _WndProc;
}
