#include "pch.h"
#include "ScriptGlobal.h"
#include "Pointers.h"

ScriptGlobal::ScriptGlobal(uintptr_t index) :
	m_Index(index)
{
}

ScriptGlobal& ScriptGlobal::At(ptrdiff_t index)
{
	m_Index += index;
	return *this;
}

ScriptGlobal& ScriptGlobal::At(ptrdiff_t index, uintptr_t size)
{
	m_Index += (index * size) + 1;
	return *this;
}

void* ScriptGlobal::GetAddress()
{
	void* Address = reinterpret_cast<void*>(Pointers::ScriptGlobals[m_Index >> 0x12 & 0x3F] + (m_Index & 0x3FFFF));
	assert(Address);
	return Address;
}
