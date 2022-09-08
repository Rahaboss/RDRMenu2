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
		//IsSessionStarted = Signature("48 83 EC 28 80 3D ? ? ? ? ? 74 12 48 8B 0D").Add(6).Rip().Get<decltype(IsSessionStarted)>();
		//FrameCount = Signature("48 89 05 ? ? ? ? E8 ? ? ? ? 8B 05 ? ? ? ? 48").Add(14).Rip().Add(4).Get<decltype(FrameCount)>();
		//SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(SwapChain)>();
		//SwapChainPresent = GetSwapChainPresent(SwapChain);
		//CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(CommandQueue)>();
		ThreadCollection = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(3).Rip().Get<decltype(ThreadCollection)>();
		RunScriptThreads = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(8).Rip().Get<decltype(RunScriptThreads)>();
		//ThreadCollection2 = Signature("48 8B 05 ? ? ? ? 49 89 0C").Add(3).Rip().Get<decltype(ThreadCollection2)>();
		//RunScriptThreads2 = Signature("73 34 48 8B 05 ? ? ? ? 8B CF 48 8B 0C").Sub(0x3D).Get<decltype(RunScriptThreads2)>();
	}
}
