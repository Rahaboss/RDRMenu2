#include "pch.h"
#include "Renderer.h"
#include "Pointers.h"
#include "Hooking.h"
#include "Menu.h"
#include "Fonts.h"
#include "Features.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Renderer
{
	static bool Setup = false;

	void Create()
	{
		TRY
		{
			LOG_TO_CONSOLE("Creating renderer.\n");
		
			if (!Pointers::SwapChain || !(*Pointers::SwapChain) || !Pointers::SwapChainPresent
				|| !Pointers::CommandQueue || !(*Pointers::CommandQueue))
			{
				LOG_TO_CONSOLE("Game is not using DirectX12 renderer backend, disabling menu renderer.\n");
				return;
			}
		
			SwapChain = *Pointers::SwapChain;
			CommandQueue = *Pointers::CommandQueue;
			Hooking::SwapChainPresent.Create(Pointers::SwapChainPresent, Hooking::SwapChainPresentHook);
		
			SetupD3D12();
			SetupImGui();

			Setup = true;
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void Destroy()
	{
		TRY
		{ 
			LOG_TO_CONSOLE("Destroying renderer.\n");
			if (!Setup)
				return;

			// Don't call ImGui shutdown functions!
			SetWindowLongPtr(Hwnd, GWLP_WNDPROC, (LONG_PTR)_WndProc);

			Hooking::SwapChainPresent.Destroy();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void Present()
	{
		TRY
		{
			if (Setup)
			{
				NewFrame();
				Features::RenderImGuiESP();
				Menu::RenderMenu();
				EndFrame();
			}
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
			ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;
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

	void SetupD3D12()
	{
		Hwnd = FindWindow(L"sgaWindow", NULL);

		if (FAILED(SwapChain->GetDevice(IID_PPV_ARGS(&Device))))
			return;

		DXGI_SWAP_CHAIN_DESC Desc;
		SwapChain->GetDesc(&Desc);
		BuffersCounts = Desc.BufferCount;
		FrameContext = new _FrameContext[BuffersCounts];

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender;
		DescriptorImGuiRender.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		DescriptorImGuiRender.NumDescriptors = BuffersCounts;
		DescriptorImGuiRender.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
		DescriptorImGuiRender.NodeMask = 0;

		if (FAILED(Device->CreateDescriptorHeap(&DescriptorImGuiRender,
			IID_PPV_ARGS(&DescriptorHeapImGuiRender))))
			return;

		ID3D12CommandAllocator* Allocator;
		if (FAILED(Device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT,
			IID_PPV_ARGS(&Allocator))))
			return;

		for (size_t i = 0; i < BuffersCounts; i++)
			FrameContext[i].CommandAllocator = Allocator;

		if (FAILED(Device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
			Allocator, NULL, IID_PPV_ARGS(&CommandList)))
			|| FAILED(CommandList->Close()))
			return;

		D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers;
		DescriptorBackBuffers.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
		DescriptorBackBuffers.NumDescriptors = BuffersCounts;
		DescriptorBackBuffers.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		DescriptorBackBuffers.NodeMask = 1;

		if (FAILED(Device->CreateDescriptorHeap(&DescriptorBackBuffers,
			IID_PPV_ARGS(&DescriptorHeapBackBuffers))))
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
	}

	void SetupImGui()
	{
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		//ImGui::StyleColorsCherry();

		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& Style = ImGui::GetStyle();

		Style.WindowTitleAlign.x = 0.5f;
		Style.WindowRounding = Style.ChildRounding = Style.FrameRounding = Style.PopupRounding =
			Style.ScrollbarRounding = Style.GrabRounding = Style.TabRounding = 4.0f;

		std::filesystem::path Path(Features::GetConfigPath());

		if (!std::filesystem::exists(Path))
		{
			std::filesystem::create_directory(Path);
		}
		else if (!std::filesystem::is_directory(Path))
		{
			std::filesystem::remove(Path);
			std::filesystem::create_directory(Path);
		}

		Path.append("imgui.ini");

		// Made static so it persists until end of the program
		static std::string IniPath(Path.string());
		io.IniFilename = IniPath.c_str();

		ImFontConfig FontCfg{};
		FontCfg.FontDataOwnedByAtlas = false;

		// Main font
		strcpy_s(FontCfg.Name, "Chalet London 1960");
		io.FontDefault = io.Fonts->AddFontFromMemoryTTF((void*)Fonts::ChaletLondon1960, sizeof(Fonts::ChaletLondon1960), 20.0f, &FontCfg);
		
		//strcpy_s(FontCfg.Name, "Redemption");
		//io.Fonts->AddFontFromMemoryTTF((void*)Fonts::Redemption, sizeof(Fonts::Redemption), 20.0f, &FontCfg);
		//
		//strcpy_s(FontCfg.Name, "RDR-Lino");
		//io.Fonts->AddFontFromMemoryTTF((void*)Fonts::RDRLino, sizeof(Fonts::RDRLino), 20.0f, &FontCfg);
		
		DefaultFont = io.Fonts->AddFontDefault();

		ImGui_ImplWin32_Init(Hwnd);
		ImGui_ImplDX12_Init(Device, BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM,
			DescriptorHeapImGuiRender,
			DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
			DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
		ImGui_ImplDX12_CreateDeviceObjects();
		_WndProc = (WNDPROC)SetWindowLongPtr(Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);
	}

	static POINT CursorCoords{};
	LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		TRY
		{
			if (uMsg == WM_KEYUP && wParam == VK_INSERT)
			{
				// Persist and restore the cursor position between menu instances
				if (Menu::IsOpen)
					GetCursorPos(&CursorCoords);
				else if (CursorCoords.x + CursorCoords.y != 0)
					SetCursorPos(CursorCoords.x, CursorCoords.y);

				Menu::IsOpen = !Menu::IsOpen;
			}
			
			if (Menu::IsOpen)
				ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam);

			// Always call the original event handler even if menu is open
			return CallWindowProc(_WndProc, hwnd, uMsg, wParam, lParam);
		}
		EXCEPT{ LOG_EXCEPTION(); }
		
		return 0;
	}
}
