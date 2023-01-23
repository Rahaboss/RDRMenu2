#include "pch.h"
#include "ScriptGlobal.h"
#include "Pointers.h"

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

void* ScriptGlobal::GetAddress()
{
	int64_t* Temp = Pointers::ScriptGlobals[m_Index >> 0x12 & 0x3F];
	//assert(Temp && "Script globals are not loaded!");
	if (!Temp)
		return nullptr;
	return reinterpret_cast<void*>(Temp + (m_Index & 0x3FFFF));
}
