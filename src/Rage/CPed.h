#pragma once

#include "pch.h"

union int128_t
{
	int64_t x[2];
};
static_assert(sizeof(int128_t) == 16);

namespace rage
{
	class CPed
	{
	public:
		virtual ~CPed() = 0; //0x0000
		char pad_0008[8]; //0x0008
		void *N0000097C; //0x0010
		char pad_0018[8]; //0x0018
		void *N0000097E; //0x0020
		char pad_0028[8]; //0x0028
		uint8_t N00000980; //0x0030
		char pad_0031[15]; //0x0031
		int128_t N00000982; //0x0040
		int128_t N00000983; //0x0050
		int128_t N00000984; //0x0060
		int128_t N00000985; //0x0070
		char pad_0080[28]; //0x0080
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
