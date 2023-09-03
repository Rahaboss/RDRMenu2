#include "pch.h"
#include "DetourHook.h"

void DetourHook::Create(void* Target, void* Detour)
{
	m_Target = Target;
	std::cout << "Creating hook 0x" << m_Target << ".\n";
	assert(m_Target && Detour);
	MH_STATUS Status = MH_CreateHook(m_Target, Detour, &m_Original);
	if (Status != MH_OK)
		std::cout << __FUNCTION__": Minhook error " << MH_StatusToString(Status) << ".\n";
	assert(Status == MH_OK);
}

void DetourHook::Destroy()
{
	std::cout << "Destroying hook 0x" << m_Target << ".\n";
	assert(m_Target);
	MH_STATUS Status = MH_RemoveHook(m_Target);
	if (Status != MH_OK)
		std::cout << __FUNCTION__": Minhook error " << MH_StatusToString(Status) << ".\n";
	assert(Status == MH_OK);
}
