#include "pch.h"
#include "Pointers.h"
#include "Signature.h"

namespace Pointers
{
	SwapChainPresent_t GetSwapChainPresent(IDXGISwapChain3** sc)
	{
		// vtbl** -> vtbl* -> present
		if (sc; void* p = *(void**)sc)
			if (p = *(void**)p)
				return ((SwapChainPresent_t*)p)[8];
		return 0;
	}

	void Scan()
	{
		LOG_TO_CONSOLE("Scanning for pointers.\n");
		IsSessionStarted = Signature("48 83 EC 28 80 3D ? ? ? ? ? 74 12 48 8B 0D").Add(6).Rip().Get<decltype(IsSessionStarted)>();
		FrameCount = Signature("48 89 05 ? ? ? ? E8 ? ? ? ? 8B 05 ? ? ? ? 48").Add(14).Rip().Add(4).Get<decltype(FrameCount)>();
		ThreadCollection = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(3).Rip().Get<decltype(ThreadCollection)>();
		RunScriptThreads = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(8).Rip().Get<decltype(RunScriptThreads)>();
		//ThreadCollection2 = Signature("48 8B 05 ? ? ? ? 49 89 0C").Add(3).Rip().Get<decltype(ThreadCollection2)>();
		//RunScriptThreads2 = Signature("73 34 48 8B 05 ? ? ? ? 8B CF 48 8B 0C").Sub(0x3D).Get<decltype(RunScriptThreads2)>();
		ActiveThread = Signature("41 C6 04 06 01 48 8D 45 08 48 89 44 24 20 E8").Add(34).Rip().Get<decltype(ActiveThread)>();
		GetNativeHandler = Signature("E8 ? ? ? ? 42 8B 9C FE").Add(1).Rip().Get<decltype(GetNativeHandler)>();
		GetPlayerPed = Signature("40 53 48 83 EC 20 33 DB 38 1D ? ? ? ? 74 1C").Get<decltype(GetPlayerPed)>();
		DebuggerCheck1 = Signature("E8 ? ? ? ? CC 48 83 EC 18").Add(1).Rip().Get<decltype(DebuggerCheck1)>();
		DebuggerCheck2 = Signature("E8 ? ? ? ? FF 15 ? ? ? ? 48 8B C8 BA ? ? ? ? 48 83 C4 28").Add(1).Rip().Get<decltype(DebuggerCheck2)>();
		DecreaseAmmo = Signature("0F 57 DB 0F 57 D2 8B").Sub(0x3D).Get<decltype(DecreaseAmmo)>();
		PedFactoryBase = Signature((uintptr_t)GetPlayerPed).Add(0x3D).Rip().Get<decltype(PedFactoryBase)>();
		PedFactoryHash = Signature((uintptr_t)GetPlayerPed).Add(0x44).Rip().Get<decltype(PedFactoryHash)>();
		uintptr_t GetBlipAddress = Signature("E8 ? ? ? ? 49 3B C7 74 4B").Add(1).Rip().GetRaw();
		BlipBase = Signature(GetBlipAddress).Add(0x1E).Rip().Get<decltype(BlipBase)>();
		BlipHash = Signature(GetBlipAddress).Add(0x25).Rip().Get<decltype(BlipHash)>();
		sub_7FF73D8C1638 = Signature("E8 ? ? ? ? 44 8A 46 16").Add(1).Rip().Get<decltype(sub_7FF73D8C1638)>();
		GetEntityPed = Signature("E8 ? ? ? ? 44 0F B6 C5").Add(1).Rip().Get<decltype(GetEntityPed)>();
		GetEntityAddress = Signature("E8 ? ? ? ? 45 8D 47 04").Add(1).Rip().Get<decltype(GetEntityAddress)>();
		dword_7FF74047DEB0 = Signature("74 30 48 69 C8 ? ? ? ? 0F 28 CE").Sub(8).Rip().Get<decltype(dword_7FF74047DEB0)>();
		qword_7FF74047DEB8 = Signature("74 30 48 69 C8 ? ? ? ? 0F 28 CE").Add(0xF).Rip().Get<decltype(qword_7FF74047DEB8)>();
		ScriptGlobals = Signature("48 8D 15 ? ? ? ? 48 8B 1D ? ? ? ? 8B").Add(3).Rip().Get<decltype(ScriptGlobals)>();
		GetStringFromHashKey = Signature("40 53 48 83 EC 40 8B D9 48 8D 15").Get<decltype(GetStringFromHashKey)>();
		WorldToScreen = Signature("E8 ? ? ? ? 0F B6 D0 48 8B 03 89 10 48 83 C4 20 5B C3 48 8B 41 10 48 8B 48 08").Add(1).Rip().Get<decltype(WorldToScreen)>();
		sub_7FF73D8DB3C4 = Signature("E8 ? ? ? ? 8A 50 28").Add(1).Rip().Get<decltype(sub_7FF73D8DB3C4)>();
		GetEntityPedReal = Signature("44 8B C9 83 F9 FF").Get<decltype(GetEntityPedReal)>();
		PedPoolEncryption = Signature("0F 28 F0 48 85 DB 74 56 8A 05 ? ? ? ? 84 C0 75 05").Add(0xA).Rip().Get<decltype(PedPoolEncryption)>();
		PedPoolBase = Signature("0F 28 F0 48 85 DB 74 56 8A 05 ? ? ? ? 84 C0 75 05").Add(0xA).Rip().Add(8).Get<decltype(PedPoolBase)>();
		PedPoolHash = Signature("0F 28 F0 48 85 DB 74 56 8A 05 ? ? ? ? 84 C0 75 05").Add(0xA).Rip().Add(0x10).Get<decltype(PedPoolHash)>();
		FwScriptGuidCreateGuid = Signature("E8 ? ? ? ? B3 01 8B 15").Sub(0x8D).Get<decltype(FwScriptGuidCreateGuid)>();

		// D3D12 Renderer Stuff
		Pointers::SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(Pointers::SwapChain)>();
		Pointers::SwapChainPresent = GetSwapChainPresent(Pointers::SwapChain);
		Pointers::CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(Pointers::CommandQueue)>();

		// Vulkan Renderer Stuff
#if ENABLE_VULKAN_RENDERER
		{
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

			ImGui_ImplVulkan_InitInfo InitInfo{};
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
		}
#endif
	}
}
