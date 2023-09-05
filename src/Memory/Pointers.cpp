#include "pch.h"
#include "Pointers.h"
#include "Signature.h"

void Pointers::Create()
{
	LOG_TEXT("Scanning for pointers.\n");

	// D3D12 renderer
	SwapChain = Signature("48 8D 15 ? ? ? ? 4C 8B 05 ? ? ? ? 4C 8D 0D").Add(3).Rip().Get<decltype(SwapChain)>();
	CommandQueue = Signature("4C 8D 0D ? ? ? ? 4C 89 65 B8").Add(3).Rip().Get<decltype(CommandQueue)>();

	// RAGE
	ThreadCollection = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(3).Rip().Get<decltype(ThreadCollection)>();
	RunScriptThreads = Signature("48 8D 0D ? ? ? ? E8 ? ? ? ? EB 0B 8B 0D").Add(8).Rip().Get<decltype(RunScriptThreads)>();
	ActiveThread = Signature("41 C6 04 06 01 48 8D 45 08 48 89 44 24 20 E8").Add(34).Rip().Get<decltype(ActiveThread)>();
	GetNativeHandler = Signature("E8 ? ? ? ? 42 8B 9C FE").Add(1).Rip().Get<decltype(GetNativeHandler)>();
	ScriptGlobals = Signature("48 8D 15 ? ? ? ? 48 8B 1D ? ? ? ? 8B").Add(3).Rip().Get<decltype(ScriptGlobals)>();
	GetPlayerPed = Signature("40 53 48 83 EC 20 33 DB 38 1D ? ? ? ? 74 1C").Get<decltype(GetPlayerPed)>();
	PedFactoryEncryption = Signature("40 53 48 83 EC 20 33 DB 38 1D ? ? ? ? 74 1C").Add(0x3D).Rip().Sub(0x10).Get<decltype(PedFactoryEncryption)>();
	DecreaseAmmo = Signature("0F 57 DB 0F 57 D2 8B").Sub(0x3D).Get<decltype(DecreaseAmmo)>();
	WorldToScreen = Signature("E8 ? ? ? ? 0F B6 D0 48 8B 03 89 10 48 83 C4 20 5B C3 48 8B 41 10 48 8B 48 08").Add(1).Rip().Get<decltype(WorldToScreen)>();
	GetPedBoneCoords = Signature("4C 8D 48 10 8B 10").Add(7).Rip().Get<decltype(GetPedBoneCoords)>();
	sub_7FF669910FEC = Signature("E8 ? ? ? ? 41 8D 7D 09").Add(1).Rip().Get<decltype(sub_7FF669910FEC)>();
	GetPedBoneIndex = Signature("E8 ? ? ? ? 8A 4D 65").Add(1).Rip().Get<decltype(GetPedBoneIndex)>();
}
