#include "pch.h"
#include "Renderer.h"
#include "Pointers.h"
#include "Hooking.h"
#include "Menu.h"
#include "Fonts.h"
#include "Features.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace ImGui
{
	void StyleColorsCherry()
	{
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text] = ImVec4(0.86f, 0.93f, 0.89f, 0.78f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		colors[ImGuiCol_ChildBg] = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.90f);
		colors[ImGuiCol_Border] = ImVec4(0.54f, 0.48f, 0.25f, 0.16f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.78f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.23f, 0.20f, 0.27f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.50f, 0.08f, 0.26f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.78f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_CheckMark] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		colors[ImGuiCol_SliderGrab] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.71f, 0.22f, 0.27f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.86f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.46f, 0.20f, 0.30f, 0.76f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.86f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.50f, 0.08f, 0.26f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.14f, 0.16f, 0.19f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.78f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.46f, 0.20f, 0.30f, 0.78f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.46f, 0.20f, 0.30f, 0.76f);
		colors[ImGuiCol_TabHovered] = ImVec4(0.50f, 0.08f, 0.26f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.50f, 0.08f, 0.26f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.14f, 0.26f, 0.42f, 1.00f);
		colors[ImGuiCol_PlotLines] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_PlotHistogram] = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
		colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.46f, 0.20f, 0.30f, 1.00f);
		colors[ImGuiCol_TableHeaderBg] = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
		colors[ImGuiCol_TableBorderStrong] = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
		colors[ImGuiCol_TableBorderLight] = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
		colors[ImGuiCol_TableRowBg] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt] = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg] = ImVec4(0.46f, 0.20f, 0.30f, 0.43f);
		colors[ImGuiCol_DragDropTarget] = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.20f, 0.22f, 0.27f, 0.73f);
	}
}

namespace Renderer
{
	static bool Setup = false;

	void Create()
	{
		TRY
		{
			printf("Creating renderer.\n");
		
			if (!Pointers::SwapChain || !(*Pointers::SwapChain))
				return;
			SwapChain = *Pointers::SwapChain;
		
			if (!Pointers::SwapChainPresent)
				return;

			CommandQueue = *Pointers::CommandQueue;
			if (!Pointers::CommandQueue || !(*Pointers::CommandQueue))
				return;

			Hooking::SwapChainPresent.Create(Pointers::SwapChainPresent,
				Hooking::SwapChainPresentHook);
		
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
			printf("Destroying renderer.\n");
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
		Desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
		Desc.OutputWindow = Hwnd;
		Desc.Windowed = !(GetWindowLongPtr(Hwnd, GWL_STYLE) & WS_POPUP);

		BuffersCounts = Desc.BufferCount;
		FrameContext = new _FrameContext[BuffersCounts];

		const D3D12_DESCRIPTOR_HEAP_DESC DescriptorImGuiRender{
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, BuffersCounts,
			D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE, 0
		};

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

		const D3D12_DESCRIPTOR_HEAP_DESC DescriptorBackBuffers{
			D3D12_DESCRIPTOR_HEAP_TYPE_RTV, BuffersCounts,
			D3D12_DESCRIPTOR_HEAP_FLAG_NONE, 1
		};

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

		ImGuiIO& io = ImGui::GetIO();
		io.IniFilename = IniPath.c_str();
		DefaultFont = io.Fonts->AddFontDefault();

		ImFontConfig FontCfg{};
		FontCfg.FontDataOwnedByAtlas = false;

		// Main font
		strcpy_s(FontCfg.Name, "Chalet London 1960");
		io.FontDefault = io.Fonts->AddFontFromMemoryTTF((void*)Fonts::ChaletLondon1960, sizeof(Fonts::ChaletLondon1960), 20.0f, &FontCfg);

		strcpy_s(FontCfg.Name, "Redemption");
		io.Fonts->AddFontFromMemoryTTF((void*)Fonts::Redemption, sizeof(Fonts::Redemption), 20.0f, &FontCfg);

		strcpy_s(FontCfg.Name, "RDR-Lino");
		io.Fonts->AddFontFromMemoryTTF((void*)Fonts::RDRLino, sizeof(Fonts::RDRLino), 20.0f, &FontCfg);

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
			return CallWindowProc((WNDPROC)_WndProc, hwnd, uMsg, wParam, lParam);
		}
		EXCEPT{ LOG_EXCEPTION(); }
		
		return 0;
	}
}
