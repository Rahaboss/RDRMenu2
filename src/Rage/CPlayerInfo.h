#pragma once

#include "pch.h"

namespace rage
{
	union netAddress
	{
		uint32_t m_Raw; //0x0000
		struct
		{
			uint8_t m_Field4;
			uint8_t m_Field3;
			uint8_t m_Field2;
			uint8_t m_Field1;
		};
	}; //Size: 0x0004
	static_assert(sizeof(netAddress) == 0x4);

	class netPlayerData
	{
	public:
		char pad_0000[8]; //0x0000
		uint64_t m_RockstarID; //0x0008
		char pad_0010[52]; //0x0010
		netAddress m_RelayIP; //0x0044
		uint16_t m_RelayPort; //0x0048
		char pad_004A[2]; //0x004A
		netAddress m_ExternalIP; //0x004C
		uint16_t m_ExternalPort; //0x0050
		char pad_0052[2]; //0x0052
		netAddress m_InternalIP; //0x0054
		uint16_t m_InternalPort; //0x0058
		char pad_005A[78]; //0x005A
		uint64_t m_RockstarID2; //0x00A8
		char pad_00B0[12]; //0x00B0
		char m_Name[20]; //0x00BC
	}; //Size: 0x00D0
	static_assert(sizeof(netPlayerData) == 0xD0);

	class CPlayerInfo
	{
	public:
		char pad_0000[336]; //0x0000
		netPlayerData m_NetPlayerData; //0x0150
		char pad_0220[4008]; //0x0220
		CPed* m_Ped; //0x11C8
		char pad_11D0[3632]; //0x11D0
	};
}
