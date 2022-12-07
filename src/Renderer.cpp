#include "pch.h"
#include "Renderer.h"
#include "Signature.h"
#include "Pointers.h"
#include "Hooking.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Renderer
{
	static bool Setup = false;

	SwapChainPresent_t GetSwapChainPresent(IDXGISwapChain3** sc)
	{
		// vtbl** -> vtbl* -> present
		if (sc; void* p = *(void**)sc)
			if (p = *(void**)p)
				return ((SwapChainPresent_t*)p)[8];
		return 0;
	}

	void Create()
	{
		std::cout << "Creating renderer.\n";

		Pointers::SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(Pointers::SwapChain)>();
		if (!Pointers::SwapChain || !(*Pointers::SwapChain))
			return;

		Pointers::SwapChainPresent = GetSwapChainPresent(Pointers::SwapChain);
		if (!Pointers::SwapChainPresent)
			return;

		Pointers::CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(Pointers::CommandQueue)>();
		if (!Pointers::CommandQueue || !(*Pointers::CommandQueue))
			return;

		Hooking::SwapChainPresent.Create(Pointers::SwapChainPresent, Hooking::SwapChainPresentHook);
		
		SetupImGui();

		Setup = true;
	}

	void Destroy()
	{
		std::cout << "Destroying renderer.\n";
		if (!Setup)
			return;

		SetWindowLongPtr(Hwnd, GWLP_WNDPROC, (LONG_PTR)_WndProc);

		Hooking::SwapChainPresent.Destroy();
	}
	
	LRESULT APIENTRY WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
		if (uMsg == WM_KEYUP && wParam == VK_INSERT)
			MenuOpen = !MenuOpen;

		if (MenuOpen) {
			ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
			return true;
		}

		return CallWindowProc(_WndProc, hwnd, uMsg, wParam, lParam);
	}

	void SetupImGui()
	{
		Hwnd = FindWindow(L"sgaWindow", NULL);

		IDXGISwapChain3* SwapChain = *Pointers::SwapChain;

		if (FAILED(SwapChain->GetDevice(IID_PPV_ARGS(&Device))))
			return;

		DXGI_SWAP_CHAIN_DESC Desc;
		SwapChain->GetDesc(&Desc);
		Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		Desc.OutputWindow = Hwnd;
		Desc.Windowed = ((GetWindowLongPtr(Hwnd, GWL_STYLE) & WS_POPUP) != 0) ? false : true;

		BuffersCounts = Desc.BufferCount;
		FrameContext = new _FrameContext[BuffersCounts];

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender = {};
		DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		DescriptorImGuiRender.NumDescriptors = BuffersCounts;
		DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

		if (FAILED(Device->CreateDescriptorHeap(&DescriptorImGuiRender, IID_PPV_ARGS(&DescriptorHeapImGuiRender))))
			return;

		ID3D12CommandAllocator* Allocator;
		if (FAILED(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&Allocator))))
			return;

		for (size_t i = 0; i < BuffersCounts; i++)
			FrameContext[i].CommandAllocator = Allocator;

		if (FAILED(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, Allocator, NULL, IID_PPV_ARGS(&CommandList)))
			|| FAILED(CommandList->Close()))
			return;

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
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
			ID3D12Resource* pBackBuffer = nullptr;
			FrameContext[i].DescriptorHandle = RTVHandle;
			SwapChain->GetBuffer(i, IID_PPV_ARGS(&pBackBuffer));
			Device->CreateRenderTargetView(pBackBuffer, NULL, RTVHandle);
			FrameContext[i].Resource = pBackBuffer;
			RTVHandle.ptr += RTVDescriptorSize;
		}

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();

		ImGui_ImplWin32_Init(Hwnd);
		ImGui_ImplDX12_Init(Device, BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM,
			DescriptorHeapImGuiRender,
			DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
			DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
		ImGui_ImplDX12_CreateDeviceObjects();
		_WndProc = (WNDPROC)SetWindowLongPtr(Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
	}

	void DestroyImGui()
	{

	}

	void Present()
	{
		NewFrame();
		
		if (MenuOpen)
		{
			ImGui::ShowDemoWindow();

			ImGui::Begin("Test Window");
			if (ImGui::Button("Exit"))
				g_Running = false;
			ImGui::End();
		}
		
		EndFrame();
	}
	
	void NewFrame()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
		ImGui::GetIO().MouseDrawCursor = MenuOpen;
	}

	void EndFrame()
	{
		_FrameContext& CurrentFrameContext = FrameContext[(*Pointers::SwapChain)->GetCurrentBackBufferIndex()];
		CurrentFrameContext.CommandAllocator->Reset();

		D3D12_RESOURCE_BARRIER Barrier;
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
		(*Pointers::CommandQueue)->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&CommandList));
	}
}
