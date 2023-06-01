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

	uintptr_t GetRaw();

	template <typename T>
	T Get()
	{
		assert(m_Result);
		return reinterpret_cast<T>(m_Result);
	}

private:
	// A 0 byte equals unknown or ?
	// technically there is no way to scan for a zero byte,
	// but there has also never been a need to, so...
	std::vector<uint8_t> m_Bytes;
	uintptr_t m_Result = 0;
};
