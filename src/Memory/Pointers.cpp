#include "pch.h"
#include "Pointers.h"
#include "Signature.h"

void Pointers::Create()
{
	LOG_TEXT("Scanning for pointers.");

	std::vector<std::function<void()>> Scans;

	// D3D12 renderer
	Scans.push_back([]() { SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(SwapChain)>(); });
	Scans.push_back([]() { CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(CommandQueue)>(); });
	
	// RAGE
	Scans.push_back([]() { ThreadCollection = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(3).Rip().Get<decltype(ThreadCollection)>(); });
	Scans.push_back([]() { RunScriptThreads = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(8).Rip().Get<decltype(RunScriptThreads)>(); });
	Scans.push_back([]() { ActiveThread = Signature("41 C6 04 06 01 48 8D 45 08 48 89 44 24 20 E8").Add(34).Rip().Get<decltype(ActiveThread)>(); });
	Scans.push_back([]() { GetNativeHandler = Signature("E8 ? ? ? ? 42 8B 9C FE").Add(1).Rip().Get<decltype(GetNativeHandler)>(); });
	Scans.push_back([]() { ScriptGlobals = Signature("48 8D 15 ? ? ? ? 48 8B 1D ? ? ? ? 8B").Add(3).Rip().Get<decltype(ScriptGlobals)>(); });
	Scans.push_back([]() { GetPlayerPed = Signature("40 53 48 83 EC 20 33 DB 38 1D ? ? ? ? 74 1C").Get<decltype(GetPlayerPed)>(); });
	Scans.push_back([]() { PedFactoryEncryption = Signature("40 53 48 83 EC 20 33 DB 38 1D ? ? ? ? 74 1C").Add(0x3D).Rip().Sub(0x10).Get<decltype(PedFactoryEncryption)>(); });
	Scans.push_back([]() { DecreaseAmmo = Signature("0F 57 DB 0F 57 D2 8B").Sub(0x3D).Get<decltype(DecreaseAmmo)>(); });
	Scans.push_back([]() { WorldToScreen = Signature("E8 ? ? ? ? 0F B6 D0 48 8B 03 89 10 48 83 C4 20 5B C3 48 8B 41 10 48 8B 48 08").Add(1).Rip().Get<decltype(WorldToScreen)>(); });
	Scans.push_back([]() { GetPedBoneCoords = Signature("4C 8D 48 10 8B 10").Add(7).Rip().Get<decltype(GetPedBoneCoords)>(); });
	Scans.push_back([]() { GetPedAddress = Signature("E8 ? ? ? ? 41 8D 7D 09").Add(1).Rip().Get<decltype(GetPedAddress)>(); });
	Scans.push_back([]() { GetPedBoneIndex = Signature("E8 ? ? ? ? 8A 4D 65").Add(1).Rip().Get<decltype(GetPedBoneIndex)>(); });
	Scans.push_back([]() { fwScriptGuidCreateGuid = Signature("E8 ? ? ? ? B3 01 8B 15").Sub(0x8D).Get<decltype(fwScriptGuidCreateGuid)>(); });
	Scans.push_back([]() { PedPoolEncryption = Signature("0F 28 F0 48 85 DB 74 56 8A 05 ? ? ? ? 84 C0 75 05").Add(0xA).Rip().Get<decltype(PedPoolEncryption)>(); });
	Scans.push_back([]() { ObjectPoolEncryption = Signature("3C 05 75 67").Add(0x14).Rip().Get<decltype(ObjectPoolEncryption)>(); });
	Scans.push_back([]() { VehiclePoolEncryption = Signature("48 83 EC 20 8A 05 ? ? ? ? 45 33 E4").Add(6).Rip().Get<decltype(VehiclePoolEncryption)>(); });
	Scans.push_back([]() { PickupPoolEncryption = Signature("0F 84 ? ? ? ? 8A 05 ? ? ? ? 48 85").Add(8).Rip().Get<decltype(PickupPoolEncryption)>(); });
	Scans.push_back([]() { DoesEntityExist = Signature("40 53 48 83 EC 20 83 F9 FF 75 04").Get<decltype(DoesEntityExist)>(); });
	Scans.push_back([]() { GetEntityModel = Signature("40 53 48 83 EC 20 33 DB E8 ? ? ? ? 48 85 C0 74 25").Get<decltype(GetEntityModel)>(); });
	Scans.push_back([]() { GetEntityCoords = Signature("48 89 5C 24 ? 57 48 83 EC 20 48 8B F9 41 8A D9").Get<decltype(GetEntityCoords)>(); });
	Scans.push_back([]() { GetStringFromHashKey = Signature("40 53 48 83 EC 40 8B D9 48 8D 15").Get<decltype(GetStringFromHashKey)>(); });
	Scans.push_back([]() { ReturnAddressSpoof = Signature("FF E3").Get<decltype(ReturnAddressSpoof)>(); });
	Scans.push_back([]() { GetMetaPedType = Signature("48 83 EC 28 85 C9 74 29").Get<decltype(GetMetaPedType)>(); });
	Scans.push_back([]() { GetEntityHealth = Signature("33 DB E8 ? ? ? ? 48 8B F8 48 85 C0 74 3C 48 8B C8").Sub(10).Get<decltype(GetEntityHealth)>(); });
	Scans.push_back([]() { IsSessionStarted = Signature("48 83 EC 28 80 3D ? ? ? ? ? 74 12 48 8B 0D").Add(6).Rip().Get<decltype(IsSessionStarted)>(); });
	Scans.push_back([]() { NetworkPlayerMgr = Signature("E8 ? ? ? ? 40 0F B6 D5").Add(1).Rip().Add(13).Rip().Get<decltype(NetworkPlayerMgr)>(); });
	Scans.push_back([]() { qword_7FF66EEBCE48 = Signature("E8 ? ? ? ? 48 03 C6").Add(1).Rip().Add(0x1B).Rip().Get<decltype(qword_7FF66EEBCE48)>(); });
	
#if ENABLE_VULKAN_RENDERER
	// Vulkan renderer
	Scans.push_back([]() { vkInstance = Signature("4C 8D 05 ? ? ? ? 33 D2 48 8D 8C 24").Add(3).Rip().Get<decltype(vkInstance)>(); });
	Scans.push_back([]() { vkPhysicalDevice = Signature("48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? B8").Add(3).Rip().Get<decltype(vkPhysicalDevice)>(); });
	Scans.push_back([]() { vkDevice = Signature("48 8B 0D ? ? ? ? 4C 8D 0D ? ? ? ? B8").Add(10).Rip().Get<decltype(vkDevice)>(); });
	Scans.push_back([]() { vkQueue = Signature("48 89 05 ? ? ? ? EB 0C 49 8B C4").Add(3).Rip().Get<decltype(vkQueue)>(); });
	Scans.push_back([]() { vkGetDeviceProcAddr = *Signature("75 22 48 8B 05 ? ? ? ? 48 8D 0D").Add(12).Rip().Get<decltype(&vkGetDeviceProcAddr)>(); });
	Scans.push_back([]() { vkQueuePresentKHR = *Signature("FF 15 ? ? ? ? 41 89 45 20").Add(2).Rip().Get<decltype(&vkQueuePresentKHR)>(); });
	Scans.push_back([]() { vkQueueSubmit = *Signature("8B D6 FF 15 ? ? ? ? 85 C0 74 10").Add(4).Rip().Get<decltype(&vkQueueSubmit)>(); });
#endif // ENABLE_VULKAN_RENDERER

	std::for_each(std::execution::par, Scans.cbegin(), Scans.cend(), [](const std::function<void()>& Scan) { Scan(); });

#if ENABLE_VULKAN_RENDERER
	LOG_TEXT("vkInstance: %llX, %llX", reinterpret_cast<uintptr_t>(vkInstance), reinterpret_cast<uintptr_t>(*vkInstance));
	LOG_TEXT("vkPhysicalDevice: %llX, %llX", reinterpret_cast<uintptr_t>(vkPhysicalDevice), reinterpret_cast<uintptr_t>(*vkPhysicalDevice));
	LOG_TEXT("vkDevice: %llX, %llX", reinterpret_cast<uintptr_t>(vkDevice), reinterpret_cast<uintptr_t>(*vkDevice));
	LOG_TEXT("vkQueue: %llX, %llX", reinterpret_cast<uintptr_t>(vkQueue), reinterpret_cast<uintptr_t>(*vkQueue));
#endif // ENABLE_VULKAN_RENDERER
}
