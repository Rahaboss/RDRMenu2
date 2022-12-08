#include "pch.h"
#include "Renderer.h"
#include "Signature.h"
#include "Pointers.h"
#include "Hooking.h"
#include "menu/Menu.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Renderer
{
	static bool Setup = false;

	void Create()
	{
		TRY
		{
			std::cout << "Creating renderer.\n";
		
			if (!Pointers::SwapChain || !(*Pointers::SwapChain))
				return;
			SwapChain = *Pointers::SwapChain;
		
			if (!Pointers::SwapChainPresent)
				return;

			CommandQueue = *Pointers::CommandQueue;
			if (!Pointers::CommandQueue || !(*Pointers::CommandQueue))
				return;

			Hooking::SwapChainPresent.Create(Pointers::SwapChainPresent, Hooking::SwapChainPresentHook);
		
			Hwnd = FindWindow(L"sgaWindow", NULL);

			if (FAILED(SwapChain->GetDevice(IID_PPV_ARGS(&Device))))
				return;

			DXGI_SWAP_CHAIN_DESC Desc;
			SwapChain->GetDesc(&Desc);
			Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
			Desc.OutputWindow = Hwnd;
			Desc.Windowed = !(GetWindowLongPtr(Hwnd, GWL_STYLE) & WS_POPUP);

			BuffersCounts = Desc.BufferCount;
			FrameContext = new _FrameContext[BuffersCounts];

			const D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender{ D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, BuffersCounts,
				D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 0 };

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

			const D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers{ D3D12_DESCRIPTOR_HEAP_TYPE_RTV, BuffersCounts, D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1 };

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
			_WndProc = SetWindowLongPtr(Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			Setup = true;
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void Destroy()
	{
		TRY
		{ 
			std::cout << "Destroying renderer.\n";
			if (!Setup)
				return;

			// Don't call ImGui shutdown functions!
			SetWindowLongPtr(Hwnd, GWLP_WNDPROC, _WndProc);

			Hooking::SwapChainPresent.Destroy();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void Present()
	{
		TRY
		{
			NewFrame();
		
			Menu::RenderMenu();
		
			EndFrame();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	
	void NewFrame()
	{
		TRY
		{
			ImGui_ImplDX12_NewFrame();
			ImGui_ImplWin32_NewFrame();
			ImGui::NewFrame();
			ImGui::GetIO().MouseDrawCursor = MenuOpen;
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void EndFrame()
	{
		TRY
		{
			_FrameContext& CurrentFrameContext = FrameContext[SwapChain->GetCurrentBackBufferIndex()];
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
			CommandQueue->ExecuteCommandLists(1, reinterpret_cast<ID3D12CommandList* const*>(&CommandList));
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	static POINT CursorCoords{};
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		TRY
		{
			if (uMsg == WM_KEYUP && wParam == VK_INSERT)
			{
				// Persist and restore the cursor position between menu instances
				if (MenuOpen)
					GetCursorPos(&CursorCoords);
				else if (CursorCoords.x + CursorCoords.y != 0)
					SetCursorPos(CursorCoords.x, CursorCoords.y);

				MenuOpen = !MenuOpen;
			}

			if (MenuOpen)
			{
				ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);
				return true;
			}

			return CallWindowProc((WNDPROC)_WndProc, hwnd, uMsg, wParam, lParam);
		}
		EXCEPT{ LOG_EXCEPTION(); }
		return false;
	}
}
