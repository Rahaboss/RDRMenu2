#pragma once

#include "pch.h"

class Signature
{
public:
	Signature(std::string_view IDAPattern);
	Signature(void* CodePattern, std::string_view Mask);
	Signature(uintptr_t Address);

	Signature& Scan();
	Signature& Add(ptrdiff_t n);
	Signature& Sub(ptrdiff_t n);
	Signature& Rip();

	// Get pointer address
	uintptr_t GetRaw() const;

	// Get by pointer
	template<typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> Get() const
	{
		return reinterpret_cast<T>(m_Result);
	}

	// Get by reference
	template<typename T>
	inline std::enable_if_t<std::is_lvalue_reference_v<T>, T> Get() const
	{
		return *reinterpret_cast<std::add_pointer_t<std::remove_reference_t<T>>>(m_Result);
	}

	// Get by value
	template<typename T>
	inline std::enable_if_t<std::is_integral_v<T>, T> Get() const
	{
		return *reinterpret_cast<T*>(m_Result);
	}

private:
	uintptr_t m_Result = 0;
	std::vector<int32_t> m_Bytes; // -1 equals unknown byte
};
