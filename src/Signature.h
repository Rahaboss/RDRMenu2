#pragma once

#include "pch.h"

class Signature
{
public:
	Signature(std::string_view ida_pattern);
	Signature(void* code_pattern, std::string_view mask);
	Signature(const uintptr_t address);
	Signature& Scan();

	Signature& Add(const ptrdiff_t& n);
	Signature& Sub(const ptrdiff_t& n);
	Signature& Rip();

	uintptr_t GetRaw();

	template <typename T>
	T Get()
	{
		assert(m_result);
		return reinterpret_cast<T>(m_result);
	}

private:
	std::vector<uint8_t> m_bytes; // a 0 byte equals unknown or ?
	uintptr_t m_result = 0;
};
