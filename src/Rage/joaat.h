#pragma once

#include "pch.h"

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

#define RAGE_JOAAT joaat

static_assert(joaat("main") == 669725655);
