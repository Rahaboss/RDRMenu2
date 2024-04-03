#include "pch.h"
#include "ScriptGlobal.h"
#include "Memory/Pointers.h"

void* ScriptGlobal::GetAddress() const
{
	if (int64_t* Pool = Pointers::ScriptGlobals[m_Index >> 0x12 & 0x3F])
		return reinterpret_cast<void*>(Pool + (m_Index & 0x3FFFF));

	// assert(Pool && "Script globals are not loaded!");
	return nullptr;
}
