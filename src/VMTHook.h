#pragma once

#include "pch.h"

class VMTHook
{
public:
	void Create(void* Object, size_t NumFuncs);
	void Destroy();

	void Hook(size_t Index, void* Function);
	void Unhook(size_t Index);

	void Enable();
	void Disable();

	template <typename T>
	T GetOriginal(size_t Index)
	{
		return static_cast<T>(m_OriginalTable[Index]);
	}
private:
	void*** m_Object;
	size_t m_NumFuncs;

	void** m_OriginalTable;
	void** m_NewTable;
};
