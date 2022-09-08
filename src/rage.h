#pragma once

#include "pch.h"

class scrThread
{
public:
	virtual ~scrThread() = 0;  //0x0000
	uint32_t m_ThreadID; //0x0008
	uint32_t m_ScriptHash; //0x000C
	uint32_t m_State; //0x0010
	uint32_t m_InstructionPtr; //0x0014
	char pad_0018[1720]; //0x0018
	char* m_pszExitMessage; //0x06D0
}; //Size: 0x06D8
static_assert(sizeof(scrThread) == 0x6D8);

class GtaThread : public scrThread
{
public:
	uint32_t m_ScriptHash; //0x06D8
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
