#include "pch.h"
#include "ScriptGlobal.h"
#include "Memory/Pointers.h"

void* ScriptGlobal::GetAddress() const
{
	int64_t* Pool = Pointers::ScriptGlobals[m_Index >> 0x12 & 0x3F];
	// assert(Pool && "Script globals are not loaded!");

	if (!Pool)
		return nullptr;

	return reinterpret_cast<void*>(Pool + (m_Index & 0x3FFFF));
}
