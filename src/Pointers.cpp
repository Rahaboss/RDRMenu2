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
		// RunScriptThreads2 = Signature("73 34 48 8B 05 ? ? ? ? 8B CF 48 8B 0C").Sub(0x3D).Get<decltype(RunScriptThreads2)>();
		ActiveThread = Signature("41 C6 04 06 01 48 8D 45 08 48 89 44 24 20 E8").Add(34).Rip().Get<decltype(ActiveThread)>();
		GetNativeHandler = Signature("E8 ? ? ? ? 42 8B 9C FE").Add(1).Rip().Get<decltype(GetNativeHandler)>();
		GetPlayerPed = Signature("40 53 48 83 EC 20 33 DB 38 1D ? ? ? ? 74 1C").Get<decltype(GetPlayerPed)>();
		DebuggerCheck1 = Signature("E8 ? ? ? ? CC 48 83 EC 18").Add(1).Rip().Get<decltype(DebuggerCheck1)>();
		DebuggerCheck2 = Signature("E8 ? ? ? ? FF 15 ? ? ? ? 48 8B C8 BA ? ? ? ? 48 83 C4 28").Add(1).Rip().Get<decltype(DebuggerCheck2)>();
		DecreaseAmmo = Signature("0F 57 DB 0F 57 D2 8B").Sub(0x3D).Get<decltype(DecreaseAmmo)>();
		PedFactoryEncryption = Signature((uintptr_t)GetPlayerPed).Add(0x3D).Rip().Sub(0x10).Get<decltype(PedFactoryEncryption)>();
		BlipEncryption = Signature("E8 ? ? ? ? 49 3B C7 74 4B").Add(1).Rip().Add(0x1E).Rip().Sub(0x10).Get<decltype(PedFactoryEncryption)>();
		GetEntityPed = Signature("E8 ? ? ? ? 44 0F B6 C5").Add(1).Rip().Get<decltype(GetEntityPed)>();
		GetEntityAddress = Signature("E8 ? ? ? ? 45 8D 47 04").Add(1).Rip().Get<decltype(GetEntityAddress)>();
		ScriptGlobals = Signature("48 8D 15 ? ? ? ? 48 8B 1D ? ? ? ? 8B").Add(3).Rip().Get<decltype(ScriptGlobals)>();
		WorldToScreen = Signature("E8 ? ? ? ? 0F B6 D0 48 8B 03 89 10 48 83 C4 20 5B C3 48 8B 41 10 48 8B 48 08").Add(1).Rip().Get<decltype(WorldToScreen)>();
		sub_7FF73D8DB3C4 = Signature("E8 ? ? ? ? 8A 50 28").Add(1).Rip().Get<decltype(sub_7FF73D8DB3C4)>();
		GetEntityPedReal = Signature("44 8B C9 83 F9 FF").Get<decltype(GetEntityPedReal)>();
		fwScriptGuidCreateGuid = Signature("E8 ? ? ? ? B3 01 8B 15").Sub(0x8D).Get<decltype(fwScriptGuidCreateGuid)>();
		PedPoolEncryption = Signature("0F 28 F0 48 85 DB 74 56 8A 05 ? ? ? ? 84 C0 75 05").Add(0xA).Rip().Get<decltype(PedPoolEncryption)>();
		ObjectPoolEncryption = Signature("3C 05 75 67").Add(0x14).Rip().Get<decltype(ObjectPoolEncryption)>();
		VehiclePoolEncryption = Signature("48 83 EC 20 8A 05 ? ? ? ? 45 33 E4").Add(6).Rip().Get<decltype(VehiclePoolEncryption)>();
		PickupPoolEncryption = Signature("0F 84 ? ? ? ? 8A 05 ? ? ? ? 48 85").Add(8).Rip().Get<decltype(PickupPoolEncryption)>();
		GetNetworkPlayer = Signature("48 83 EC 28 33 C0 38 05 ? ? ? ? 74 0A").Get<decltype(GetNetworkPlayer)>();
		NetworkPlayerMgr = Signature("E8 ? ? ? ? 40 0F B6 D5").Add(1).Rip().Add(13).Rip().Get<decltype(NetworkPlayerMgr)>();
		ReturnAddressSpoof = Signature("FF E3").Get<decltype(ReturnAddressSpoof)>();

		// D3D12 Renderer Stuff
		SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(SwapChain)>();
		SwapChainPresent = GetSwapChainPresent(SwapChain);
		CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(CommandQueue)>();
	}
}
