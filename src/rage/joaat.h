#pragma once

#include "pch.h"

typedef uint32_t joaat_t;

constexpr char joaat_to_lower(char c)
{
	return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

constexpr joaat_t joaat_constexpr(const char* str)
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

#define RAGE_JOAAT joaat_constexpr