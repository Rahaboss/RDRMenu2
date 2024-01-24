#include "pch.h"
#include "DetourHook.h"

void DetourHook::Create(void* Target, void* Detour)
{
	m_Target = Target;

	LOG_TEXT("Creating hook 0x%llX.", reinterpret_cast<uintptr_t>(m_Target));

	assert(m_Target && Detour);

	MH_STATUS Status = MH_CreateHook(m_Target, Detour, &m_Original);
	if (Status != MH_OK)
		LOG_TEXT("%s: Minhook error %s.", __FUNCTION__, MH_StatusToString(Status));

	assert(Status == MH_OK);
}

void DetourHook::Destroy()
{
	LOG_TEXT("Destroying hook 0x%llX.", reinterpret_cast<uintptr_t>(m_Target));

	assert(m_Target);

	MH_STATUS Status = MH_RemoveHook(m_Target);
	if (Status != MH_OK)
		LOG_TEXT("%s: Minhook error %s.", __FUNCTION__, MH_StatusToString(Status));

	assert(Status == MH_OK);
}
