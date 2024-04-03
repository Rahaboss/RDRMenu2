#pragma once

#include "pch.h"

namespace rage
{
	class CPed
	{
	public:
		virtual ~CPed() = 0; //0x0000
		char pad_0000[216]; //0x0008
		void* N00000996; //0x00E0
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
