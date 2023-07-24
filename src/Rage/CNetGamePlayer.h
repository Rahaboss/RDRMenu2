#pragma once

#include "pch.h"

namespace rage
{
	union netAddress
	{
		uint32_t m_Raw;
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
		union netAddress m_RelayIP; //0x0044
		uint16_t m_RelayPort; //0x0048
		char pad_004A[2]; //0x004A
		union netAddress m_ExternalIP; //0x004C
		uint16_t m_ExternalPort; //0x0050
		char pad_0052[2]; //0x0052
		union netAddress m_InternalIP; //0x0054
		uint16_t m_InternalPort; //0x0058
		char pad_005A[78]; //0x005A
		uint64_t m_RockstarID2; //0x00A8
		char pad_00B0[12]; //0x00B0
		char m_Name[20]; //0x00BC
	}; //Size: 0x00D0
	static_assert(sizeof(netPlayerData) == 0xD0);

	class netPlayer
	{
	public:
		virtual ~netPlayer() = 0;
		virtual void m_0x08() = 0;
		virtual void m_0x10() = 0;
		virtual void Reset() = 0;
		virtual bool IsValid() const = 0; // *(BYTE*)(CNetGamePlayer + 0x19) < 32
		virtual const char* GetName() const = 0; // netPlayerData + 0xBC
		virtual void m_0x30() = 0;
		virtual void m_0x38() = 0;
		virtual void m_0x40() = 0;
		virtual bool IsHost() = 0; // (CNetGamePlayer + 0x118) && sub_7FF6C57C3BE8(this, 0)
		virtual void m_0x50() = 0;
		virtual void m_0x58() = 0;
		virtual netPlayerData* GetNetPlayerData() = 0; // CNetGamePlayer + 0x150
	}; //Size: 0x0008
	static_assert(sizeof(netPlayer) == 0x8);

	class CNetGamePlayer : public netPlayer
	{
	public:
		char pad_0008[17]; //0x0008
		uint8_t m_PlayerID; //0x0019
		char pad_001A[270]; //0x001A
		class CPlayerInfo* m_PlayerInfo; //0x0128
		char pad_0130[32]; //0x0130
		class netPlayerData m_NetPlayerData; //0x0150
		char pad_0220[2192]; //0x0220
	}; //Size: 0x0AB0
	static_assert(sizeof(CNetGamePlayer) == 0xAB0);
}
