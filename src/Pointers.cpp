#include "pch.h"
#include "Pointers.h"
#include "Signature.h"

void Pointers::Create()
{
	std::cout << "Scanning for pointers.\n";

	// D3D12 renderer
	SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(SwapChain)>();
	CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(CommandQueue)>();
}
