#pragma once

#include "pch.h"

namespace rage
{
	class CPed
	{
	public:
		virtual ~CPed() = 0; //0x0000
		char pad_0008[148]; //0x0008
		uint32_t m_9C; //0x009C
		char pad_00A0[64]; //0x00A0
		void* m_E0; //0x00E0
	}; //Size: 0x00E8
	static_assert(sizeof(CPed) == 0xE8);

	class CPedFactory
	{
	public:
		virtual ~CPedFactory() = 0; //0x0000
		CPed* m_LocalPed; //0x0008

		static CPedFactory* Get();
		static CPed* GetLocalPed();
	}; //Size: 0x0010
	static_assert(sizeof(CPedFactory) == 0x10);
}
