#include "pch.h"
#include "Renderer.h"
#include "Pointers.h"
#include "Hooking.h"
#include "Menu.h"
#include "Fonts.h"
#include "Features.h"
#include "Signature.h"

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

namespace Renderer
{
	static bool Setup = false;
	static bool UseD3D12 = false;

	void Create()
	{
		TRY
		{
			Hwnd = FindWindow(L"sgaWindow", NULL);

			if (Pointers::SwapChain && (*Pointers::SwapChain) && Pointers::SwapChainPresent && Pointers::CommandQueue && (*Pointers::CommandQueue))
				CreateD3D12();
			else
			{
#if ENABLE_VULKAN_RENDERER
				CreateVulkan();
#else
				LOG_TO_CONSOLE("Vulkan renderer is currently disabled, use D3D12.\n");
#endif
			}

			if (!Setup)
				LOG_TO_CONSOLE("Failed to create %s renderer!\n", (UseD3D12 ? "D3D12" : "Vulkan"));
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void CreateD3D12()
	{
		TRY
		{
			LOG_TO_CONSOLE("Creating D3D12 renderer.\n");

			UseD3D12 = true;

			SwapChain = *Pointers::SwapChain;
			CommandQueue = *Pointers::CommandQueue;

			Hooking::SwapChain.Create(SwapChain, Hooking::SwapChainMethodCount);
			Hooking::SwapChain.Hook(Hooking::SwapChainPresentIndex, Hooking::SwapChainPresentHook);
			Hooking::SwapChain.Enable();

			SetupD3D12();
			SetupImGui();

			Setup = true;
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void CreateVulkan()
	{
		TRY
		{
			LOG_TO_CONSOLE("Creating Vulkan renderer.\n");

			UseD3D12 = false;

			SetupVulkan();
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

			// Don't call ImGui shutdown functions! (at least for D3D12)
			SetWindowLongPtr(Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProc);

			if (UseD3D12)
			{
				Hooking::SwapChain.Disable();
				Hooking::SwapChain.Destroy();
			}
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
				Features::RGBTick();
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

	// Don't use TRY/EXCEPT macros in these SetupXXX() functions!
	void SetupD3D12()
	{
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

	void SetupVulkan()
	{
		ImGui_ImplVulkan_InitInfo InitInfo{};

		auto h = GetModuleHandle(L"vulkan-1.dll");
		if (!h) return;
		LOG_TXT("vulkan-1.dll: 0x%llX", h);

		auto vkGetDeviceProcAddr = (void* (*)(void*, void*))(GetProcAddress(h, "vkGetDeviceProcAddr"));
		if (!vkGetDeviceProcAddr) return;
		LOG_TXT("vkGetDeviceProcAddr: 0x%llX", vkGetDeviceProcAddr);

		auto vkInstance = Signature("4C 8D 05 ? ? ? ? 33 D2 48 8D 8C 24").Add(3).Rip().Get<VkInstance*>();
		if (!vkInstance) return;
		LOG_TXT("vkInstance: 0x%llX", vkInstance);

		auto vkPhysicalDevice = Signature("48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? B8").Add(3).Rip().Get<VkPhysicalDevice*>();
		if (!vkPhysicalDevice) return;
		LOG_TXT("vkPhysicalDevice: 0x%llX", vkPhysicalDevice);

		auto vkDevice = Signature("48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? B8").Add(10).Rip().Get<VkDevice*>();
		if (!vkDevice) return;
		LOG_TXT("vkDevice: 0x%llX", vkDevice);

		auto vkQueueFamily = 0; // Probably 0 or 1

		auto vkQueue = Signature("48 89 05 ? ? ? ? EB 0C 49 8B C4").Add(3).Rip().Get<VkQueue*>();
		if (!vkQueue) return;
		LOG_TXT("vkQueue: 0x%llX", vkQueue);

		auto vkPipelineCache = VK_NULL_HANDLE;
		auto vkDescriptorPool = VK_NULL_HANDLE;
		auto vkSubpass = NULL;
		auto vkMinImageCount = 2;
		auto vkImageCount = VK_NULL_HANDLE;
		auto vkMSAASamples = VK_NULL_HANDLE;
		auto vkAllocator = VK_NULL_HANDLE;
		static auto vkCheckFn = [](VkResult err) {
			if (err == 0)
				return;
			fprintf(stderr, "[vulkan] Error: VkResult = %d\n", err);
			if (err < 0)
				abort();
		};

		auto qword_7FF7421B1A30 = Signature("48 8B 05 ? ? ? ? 48 8B 08 0F 10 41 10 48 8B 51 08 48 8B 01").Add(3).Rip().GetRaw();
		if (!qword_7FF7421B1A30 || !(*(uintptr_t*)qword_7FF7421B1A30 + 80)) return;
		LOG_TXT("qword_7FF7421B1A30: 0x%llX", qword_7FF7421B1A30);

		auto msaa = *(uintptr_t*)(*(uintptr_t*)qword_7FF7421B1A30 + 80);
		LOG_TXT("msaa: 0x%llX", msaa);
		LOG_TXT("msaa2: 0x%llX", (uint8_t)msaa);

		auto sub_7FF73D042E94 = [](uint8_t a1) -> int64_t {
			if (a1 > 7u)
			{
				if (a1 < 8u)
					return 0i64;
				if (a1 <= 9u)
					return 3i64;
				if (a1 <= 13u)
					return 4i64;
				if (a1 != 14)
				{
					if (a1 == 15)
						return 2i64;
					if (a1 != 16)
					{
						if (a1 != 17)
							return 0i64;
						return 4i64;
					}
					return 3i64;
				}
				return 1i64;
			}
			switch (a1)
			{
			case 7u:
				return 2i64;
			case 0u:
				return 0i64;
			case 1u:
				return 1i64;
			case 2u:
				return 2i64;
			}
			if (a1 != 3)
			{
				if (a1 != 4)
				{
					if (a1 != 5)
					{
						if (a1 != 6)
							return 0i64;
						return 3i64;
					}
					return 2i64;
				}
				return 4i64;
			}
			return 3i64;
		};
		LOG_TXT("msaa3: 0x%llX", sub_7FF73D042E94((uint8_t)msaa));

		InitInfo.Instance = *vkInstance;
		InitInfo.PhysicalDevice = *vkPhysicalDevice;
		InitInfo.Device = *vkDevice;
		InitInfo.QueueFamily = vkQueueFamily;
		InitInfo.Queue = *vkQueue;
		InitInfo.PipelineCache = vkPipelineCache;
		InitInfo.DescriptorPool;
		InitInfo.Subpass = vkSubpass;
		InitInfo.MinImageCount = vkMinImageCount;
		InitInfo.ImageCount;
		InitInfo.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
		InitInfo.Allocator = vkAllocator;
		InitInfo.CheckVkResultFn = vkCheckFn;
			
		ImGui_ImplVulkan_Init(&InitInfo, VK_NULL_HANDLE);
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

		// Made static so it persists until end of the program
		static std::string IniPath(Features::GetConfigPath().append("imgui.ini").string());
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

		if (UseD3D12)
		{
			ImGui_ImplDX12_Init(Device, BuffersCounts, DXGI_FORMAT_R8G8B8A8_UNORM,
				DescriptorHeapImGuiRender,
				DescriptorHeapImGuiRender->GetCPUDescriptorHandleForHeapStart(),
				DescriptorHeapImGuiRender->GetGPUDescriptorHandleForHeapStart());
			ImGui_ImplDX12_CreateDeviceObjects();
		}
		else
		{
			//
		}

		WndProc = (WNDPROC)SetWindowLongPtr(Hwnd, GWLP_WNDPROC, (LONG_PTR)WndProcHook);
	}

	static POINT CursorCoords{};
	LRESULT CALLBACK WndProcHook(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
			return CallWindowProc(WndProc, hwnd, uMsg, wParam, lParam);
		}
		EXCEPT{ LOG_EXCEPTION(); }
		
		return 0;
	}
}
