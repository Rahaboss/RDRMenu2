#pragma once

#include "pch.h"

class ScriptGlobal
{
public:
	ScriptGlobal(uintptr_t index);
	ScriptGlobal& At(ptrdiff_t index);
	ScriptGlobal& At(ptrdiff_t index, uintptr_t size);

	// Get by pointer
	template <typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> Get()
	{
		return static_cast<T>(GetAddress());
	}

	// Get by value
	template <typename T>
	inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> Get()
	{
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(GetAddress());
	}

private:
	void* GetAddress();
	uintptr_t m_Index;
};
