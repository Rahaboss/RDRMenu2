#include "pch.h"
#include "VMTHook.h"

void VMTHook::Create(void* Object, size_t NumFuncs)
{
	m_Object = static_cast<void***>(Object);
	LOG_TEXT("Creating hook 0x%llX.", reinterpret_cast<uintptr_t>(m_Object));
	m_NumFuncs = NumFuncs;
	m_OriginalTable = *m_Object;
	m_NewTable = new void* [m_NumFuncs];
	const rsize_t TableSize = sizeof(void*) * m_NumFuncs;
	memcpy_s(m_NewTable, TableSize, m_OriginalTable, TableSize);
}

void VMTHook::Destroy()
{
	LOG_TEXT("Destroying hook 0x%llX.", reinterpret_cast<uintptr_t>(m_Object));
	delete m_NewTable;
}

void VMTHook::Hook(size_t Index, void* Function)
{
	m_NewTable[Index] = Function;
}

void VMTHook::Unhook(size_t Index)
{
	m_NewTable[Index] = m_OriginalTable[Index];
}

void VMTHook::Enable()
{
	*m_Object = m_NewTable;
}

void VMTHook::Disable()
{
	*m_Object = m_OriginalTable;
}
