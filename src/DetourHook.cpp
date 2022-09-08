#include "pch.h"
#include "Console.h"
#include "DetourHook.h"

void DetourHook::Create(void* target, void* detour)
{
	assert(target && detour && MH_CreateHook(target, detour, &m_original) == MH_OK);
	m_target = target;
}

void DetourHook::Destroy()
{
	assert(m_target && MH_RemoveHook(m_target) == MH_OK);
}
