#pragma once

#include "pch.h"

namespace rage
{
	typedef uint32_t joaat_t;

	constexpr char joaat_to_lower(char c)
	{
		return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
	}

	constexpr joaat_t joaat(const char* str)
	{
		joaat_t hash = 0;

		while (*str)
		{
			hash += joaat_to_lower(*(str++));
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}

	inline joaat_t joaat(const std::string& str)
	{
		joaat_t hash = 0;

		for (const char& c : str)
		{
			hash += joaat_to_lower(c);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
}

// Dont ask...
template <size_t CharCount>
class ConstexprJoaat
{
public:
	char Data[CharCount];

	template <size_t... Indices>
	constexpr ConstexprJoaat(const char* str, std::index_sequence<Indices...>) :
		Data{ (str[Indices])... }
	{
	}

	constexpr rage::joaat_t operator()()
	{
		rage::joaat_t hash = 0;

		for (size_t i = 0; i < CharCount; ++i)
		{
			hash += rage::joaat_to_lower(Data[i]);
			hash += (hash << 10);
			hash ^= (hash >> 6);
		}

		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);

		return hash;
	}
};

#define RAGE_JOAAT(str) (std::integral_constant<rage::joaat_t, (ConstexprJoaat<sizeof(str) - 1>((str), std::make_index_sequence<sizeof(str) - 1>())())>::value)

static_assert(RAGE_JOAAT("main") == 669725655);
