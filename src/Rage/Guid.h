#pragma once

#include "pch.h"
#include "scrNativeCallContext.h"

template<typename size_t Len>
class Guid
{
public:
	static_assert(Len > 0);

	inline constexpr size_t size() const { return Len; }

	inline constexpr Guid()
	{
		for (size_t i = 0; i < size(); i++)
			m_Buffer[i] = 0;
	}

	inline constexpr uint32_t& operator[](size_t i)
	{
		return *reinterpret_cast<uint32_t*>(&m_Buffer[i]);
	}

	inline constexpr Any* get() { return reinterpret_cast<Any*>(&m_Buffer); }

private:
	alignas(8) uint64_t m_Buffer[Len];
};
static_assert(sizeof(Guid<5>) == sizeof(uint64_t) * 5);
