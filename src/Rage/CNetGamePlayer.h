#pragma once

#include "pch.h"
#include "CPlayerInfo.h"

namespace rage
{
	class netPlayer
	{
	public:
		virtual ~netPlayer() = 0; //0x0000
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
		virtual bool m_0x58() = 0;
		virtual netPlayerData* GetNetPlayerData() const = 0; // CNetGamePlayer + 0x150
		virtual void m_0x68() = 0;
		virtual void m_0x70() = 0;
		virtual void m_0x78() = 0;
	}; //Size: 0x0008
	static_assert(sizeof(netPlayer) == 0x8);

	class CNetGamePlayer : public netPlayer
	{
	public:
		char pad_0000[280]; //0x0008
		class CNetworkPlayerMgr* m_NetworkPlayerMgr; //0x0120
		CPlayerInfo* m_PlayerInfo; //0x0128
		char pad_0130[2432]; //0x0130
	}; //Size: 0x0AB0
	static_assert(sizeof(CNetGamePlayer) == 0xAB0);
}