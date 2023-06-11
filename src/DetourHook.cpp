#include "pch.h"
#include "DetourHook.h"

void DetourHook::Create(void* Target, void* Detour)
{
	m_Target = Target;
	LOG_TO_CONSOLE("Creating hook 0x%llX.\n", (uint64_t)m_Target);
	assert(m_Target && Detour);
	MH_STATUS Status = MH_CreateHook(m_Target, Detour, &m_Original);
	if (Status != MH_OK)
		LOG_TO_CONSOLE("%s: Minhook error %s.\n", __FUNCTION__, MH_StatusToString(Status));
	assert(Status == MH_OK);
	s_HookCount++;
}

void DetourHook::Destroy()
{
	LOG_TO_CONSOLE("Destroying hook 0x%llX.\n", (uint64_t)m_Target);
	assert(m_Target);
	MH_STATUS Status = MH_RemoveHook(m_Target);
	if (Status != MH_OK)
		LOG_TO_CONSOLE("%s: Minhook error %s.\n", __FUNCTION__, MH_StatusToString(Status));
	assert(Status == MH_OK);
	s_HookCount--;
}
