#include "pch.h"
#include "Pointers.h"
#include "Signature.h"

namespace Pointers
{
	SwapChainPresent_t GetSwapChainPresent(IDXGISwapChain3** sc)
	{
		TRY
		{
			// vtbl** -> vtbl* -> present
			if (sc; void* p = *(void**)sc)
				if (p = *(void**)p)
					return ((SwapChainPresent_t*)p)[8];
		}
		EXCEPT{ LOG_EXCEPTION(); }
		return 0;
	}

	void Scan()
	{
		printf("Scanning for pointers.\n");
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

		// D3D12 Renderer Stuff
		Pointers::SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(Pointers::SwapChain)>();
		Pointers::SwapChainPresent = GetSwapChainPresent(Pointers::SwapChain);
		Pointers::CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(Pointers::CommandQueue)>();
	}
}
