#include "pch.h"
#include "RendererD3D12.h"
#include "Renderer.h"
#include "Memory/Hooking.h"
#include "Memory/Pointers.h"
#include "Menu/Menu.h"
#include "ESP/ESP.h"

void RendererD3D12::Create()
{
	SwapChain = *Pointers::SwapChain;
	CommandQueue = *Pointers::CommandQueue;

	Hooking::SwapChain.Create(SwapChain, Hooking::SwapChainMethodCount);
	Hooking::SwapChain.Hook(Hooking::SwapChainPresentIndex, Hooking::SwapChainPresentHook);
	Hooking::SwapChain.Enable();

	if (FAILED(SwapChain->GetDevice(IID_PPV_ARGS(&Device))))
		return;

	DXGI_SWAP_CHAIN_DESC Desc{};
	SwapChain->GetDesc(&Desc);
	BuffersCounts = Desc.BufferCount;
	FrameContext = new _FrameContext[BuffersCounts];

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender{};
	DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	DescriptorImGuiRender.NumDescriptors = BuffersCounts;
	DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	DescriptorImGuiRender.NodeMask = 0;

	if (FAILED(Device->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&DescriptorHeapImGuiRender))))
		return;

	ID3D12CommandAllocator* Allocator{};
	if (FAILED(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator))))
		return;

	for (size_t i = 0; i < BuffersCounts; i++)
		FrameContext[i].CommandAllocator = Allocator;

	if (FAILED(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&CommandList))) || FAILED(CommandList->Close()))
		return;

	D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers{};
	DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	DescriptorBackBuffers.NumDescriptors = BuffersCounts;
	DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	DescriptorBackBuffers.NodeMask = 1;

	if (FAILED(Device->CreateDescriptorHeap(&DescriptorBackBuffers, IID_PPV_ARGS(&DescriptorHeapBackBuffers))))
		return;

	const UINT RTVDescriptorSize = Device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	D3D12_CPU_DESCRIPTOR_HANDLE RTVHandle = DescriptorHeapBackBuffers->GetCPUDescriptorHandleForHeapStart();

	for (UINT i = 0; i < BuffersCounts; i++)
	{
		FrameContext[i].DescriptorHandle = RTVHandle;
		ID3D12Resource* pBackBuffer;
		SwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
		Device->CreateRenderTargetView(pBackBuffer, NULL, RTVHandle);
		FrameContext[i].Resource = pBackBuffer;
		RTVHandle.ptr += RTVDescriptorSize;
	}

	ImGui_ImplDX12_Init(Device, BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM,
		DescriptorHeapImGuiRender,
		DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
		DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
	ImGui_ImplDX12_CreateDeviceObjects();
}

void RendererD3D12::Destroy()
{
	Hooking::SwapChain.Disable();
	Hooking::SwapChain.Destroy();
}

void RendererD3D12::Present(IDXGISwapChain3* SwapChain)
{
	if (Renderer::Setup)
	{
		NewFrame();

		Menu::RenderMenu();
		ESP::RenderESP();

		EndFrame();
	}
}

void RendererD3D12::NewFrame()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void RendererD3D12::EndFrame()
{
	_FrameContext& CurrentFrameContext = FrameContext[SwapChain->GetCurrentBackBufferIndex()];
	CurrentFrameContext.CommandAllocator->Reset();

	D3D12_RESOURCE_BARRIER Barrier{};
	Barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	Barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	Barrier.Transition.pResource = CurrentFrameContext.Resource;
	Barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	CommandList->Reset(CurrentFrameContext.CommandAllocator, nullptr);
	CommandList->ResourceBarrier(1, &Barrier);
	CommandList->OMSetRenderTargets(1, &CurrentFrameContext.DescriptorHandle, FALSE, nullptr);
	CommandList->SetDescriptorHeaps(1, &DescriptorHeapImGuiRender);

	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), CommandList);

	Barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	Barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	CommandList->ResourceBarrier(1, &Barrier);
	CommandList->Close();
	CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&CommandList));
}
