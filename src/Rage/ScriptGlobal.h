#pragma once

#include "pch.h"

class ScriptGlobal
{
public:
	// Global_123 (Index = 123)
	inline constexpr ScriptGlobal(uintptr_t Index) :
		m_Index(Index)
	{
	}

	// Global_123.f_4 (Index = 4)
	inline constexpr ScriptGlobal& At(ptrdiff_t Index)
	{
		m_Index += Index;
		return *this;
	}

	// Global_123[4 /*5*/] (Index = 4, Size = 5)
	inline constexpr ScriptGlobal& At(ptrdiff_t Index, uintptr_t Size)
	{
		m_Index += (Index * Size) + 1;
		return *this;
	}

	// Get by pointer
	template<typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> Get() const
	{
		return static_cast<T>(GetAddress());
	}

	// Get by reference
	template<typename T>
	inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> Get() const
	{
		return *static_cast<std::add_pointer_t<std::remove_reference_t<T>>>(GetAddress());
	}

	// Get by value
	template<typename T>
	inline std::enable_if_t<std::is_integral_v<T>, T> Get() const
	{
		return *static_cast<T*>(GetAddress());
	}

private:
	void* GetAddress() const;

	uintptr_t m_Index;
};
