#include "pch.h"
#include "ScriptGlobal.h"
#include "Memory/Pointers.h"

ScriptGlobal::ScriptGlobal(uintptr_t Index) :
	m_Index(Index)
{
}

ScriptGlobal& ScriptGlobal::At(ptrdiff_t Index)
{
	m_Index += Index;
	return *this;
}

ScriptGlobal& ScriptGlobal::At(ptrdiff_t Index, uintptr_t Size)
{
	m_Index += (Index * Size) + 1;
	return *this;
}

void* ScriptGlobal::GetAddress() const
{
	int64_t* Pool = Pointers::ScriptGlobals[m_Index >> 0x12 & 0x3F];
	assert(Pool && "Script globals are not loaded!");

	if (!Pool)
		return nullptr;

	return reinterpret_cast<void*>(Pool + (m_Index & 0x3FFFF));
}
