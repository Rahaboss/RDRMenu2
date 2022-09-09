#pragma once

#include "pch.h"

typedef uint32_t joaat_t;

inline constexpr char joaat_to_lower(char c)
{
	return (c >= 'A' && c <= 'Z') ? c + ('a' - 'A') : c;
}

inline constexpr joaat_t joaat_constexpr(const char* str)
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

class scrThread
{
public:
	virtual ~scrThread() = 0;  //0x0000
	uint32_t m_ThreadID; //0x0008
	joaat_t m_ScriptHash; //0x000C
	uint32_t m_State; //0x0010
private:
	uint32_t m_InstructionPtr; //0x0014
	char pad_0018[1720]; //0x0018
	char* m_pszExitMessage; //0x06D0
}; //Size: 0x06D8
static_assert(sizeof(scrThread) == 0x6D8);

class GtaThread : public scrThread
{
public:
	joaat_t m_ScriptHash2; //0x06D8
	char pad_06DC[4]; //0x06DC
	char N0000030B[60]; //0x06E0
	char pad_071C[4]; //0x071C
	void* m_ScriptHandler; //0x0720
	char pad_0728[96]; //0x0728
}; //Size: 0x0788
static_assert(sizeof(GtaThread) == 0x788);

class pgPtrCollection
{
public:
	class GtaThread** m_pData; //0x0000
	uint16_t m_pCount; //0x0008
private:
	uint16_t m_pSize; //0x000C
}; //Size: 0x0010
static_assert(sizeof(pgPtrCollection) == 0x10);
