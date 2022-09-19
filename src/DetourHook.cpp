#include "pch.h"
#include "Console.h"
#include "DetourHook.h"

void DetourHook::Create(void* target, void* detour)
{
	m_target = target;
	std::cout << "Creating hook " << LOG_HEX(m_target) << ".\n";
	assert(m_target && detour);
	MH_STATUS status = MH_CreateHook(m_target, detour, &m_original);
	if (status != MH_OK)
		std::cout << "Minhook error: " << MH_StatusToString(status) << '\n';
	assert(status == MH_OK);
}

void DetourHook::Destroy()
{
	std::cout << "Destroying hook " << LOG_HEX(m_target) << ".\n";
	assert(m_target && MH_RemoveHook(m_target) == MH_OK);
}
