#include "pch.h"
#include "Pointers.h"
#include "Signature.h"

namespace Pointers
{
	SwapChainPresent_t GetSwapChainPresent(IDXGISwapChain1** sc)
	{
		// vtbl** -> vtbl* -> present
		if (sc; void* p = *(void**)sc)
			if (p = *(void**)p)
				return ((SwapChainPresent_t*)p)[8];
		return 0;
	}

	void Scan()
	{
		std::cout << "Scanning for pointers.\n";
		IsSessionStarted = Signature("48 83 EC 28 80 3D ? ? ? ? ? 74 12 48 8B 0D").Add(6).Rip().Get<decltype(IsSessionStarted)>();
		FrameCount = Signature("48 89 05 ? ? ? ? E8 ? ? ? ? 8B 05 ? ? ? ? 48").Add(14).Rip().Add(4).Get<decltype(FrameCount)>();
		//SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(SwapChain)>();
		//SwapChainPresent = GetSwapChainPresent(SwapChain);
		//CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(CommandQueue)>();
		ThreadCollection = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(3).Rip().Get<decltype(ThreadCollection)>();
		RunScriptThreads = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(8).Rip().Get<decltype(RunScriptThreads)>();
		//ThreadCollection2 = Signature("48 8B 05 ? ? ? ? 49 89 0C").Add(3).Rip().Get<decltype(ThreadCollection2)>();
		//RunScriptThreads2 = Signature("73 34 48 8B 05 ? ? ? ? 8B CF 48 8B 0C").Sub(0x3D).Get<decltype(RunScriptThreads2)>();
		ActiveThread = Signature("41 C6 04 06 01 48 8D 45 08 48 89 44 24 20 E8").Add(34).Rip().Get<decltype(ActiveThread)>();
		GetNativeHandler = Signature("E8 ? ? ? ? 42 8B 9C FE").Add(1).Rip().Get<decltype(GetNativeHandler)>();
		GetPlayerPed = Signature("40 53 48 83 EC 20 33 DB 38 1D ? ? ? ? 74 1C").Get<decltype(GetPlayerPed)>();
		DebuggerCheck1 = Signature("E8 ? ? ? ? CC 48 83 EC 18").Add(1).Rip().Get<decltype(DebuggerCheck1)>();
		DebuggerCheck2 = Signature("E8 ? ? ? ? FF 15 ? ? ? ? 48 8B C8 BA ? ? ? ? 48 83 C4 28").Add(1).Rip().Get<decltype(DebuggerCheck2)>();
	}
}
