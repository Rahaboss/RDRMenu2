#pragma once

#include "pch.h"

namespace Renderer
{
	void Create();
	void Destroy();
	void SetupImGui();
	void DestroyImGui();
	void Present();
	void NewFrame();
	void EndFrame();
	
	inline bool MenuOpen = true;

	struct _FrameContext
	{
		ID3D12CommandAllocator* CommandAllocator;
		ID3D12Resource* Resource;
		D3D12_CPU_DESCRIPTOR_HANDLE DescriptorHandle;
	};

	inline ID3D12Device* Device;
	inline HWND Hwnd;
	inline UINT BuffersCounts;
	inline _FrameContext* FrameContext;
	inline ID3D12DescriptorHeap* DescriptorHeapImGuiRender;
	inline ID3D12GraphicsCommandList* CommandList;
	inline ID3D12DescriptorHeap* DescriptorHeapBackBuffers;
	inline WNDPROC _WndProc;
}
