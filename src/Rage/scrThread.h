#pragma once

#include "pch.h"
#include "joaat.h"

namespace rage
{
	class scrThread
	{
	public:
		virtual ~scrThread() = 0; //0x0000
		uint32_t m_ThreadID; //0x0008
		joaat_t m_ScriptHash; //0x000C

		enum eThreadState
		{
			ThreadStateIdle,
			ThreadStateRunning,
			ThreadStateKilled,
			ThreadState3,
			ThreadState4,
		};

		eThreadState m_State; //0x0010
	private:
		char pad_0018[1732]; //0x0014
	}; //Size: 0x06D8
	static_assert(sizeof(scrThread) == 0x6D8);

	class GtaThread : public scrThread
	{
	public:
		joaat_t m_ScriptHash2; //0x06D8
	private:
		char pad_06DC[172]; //0x06DC
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
}
