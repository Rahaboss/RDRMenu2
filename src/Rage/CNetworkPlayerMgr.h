#pragma once

#include "pch.h"
#include "CNetGamePlayer.h"

namespace rage
{
	class CNetworkPlayerMgr
	{
	public:
		virtual ~CNetworkPlayerMgr() = 0; //0x0000
		virtual void m_0x08() = 0;
		virtual void m_0x10() = 0;
		virtual void m_0x18() = 0;
		virtual void m_0x20() = 0;
		virtual void m_0x28() = 0;
		virtual void m_0x30() = 0;
		virtual void m_0x38() = 0;
		virtual void m_0x40() = 0;
		virtual void m_0x48() = 0;
		virtual void m_0x50() = 0;
		virtual void m_0x58() = 0;
		virtual void m_0x60() = 0;
		virtual void m_0x68() = 0;
		virtual void m_0x70() = 0;

		char pad_0000[224]; //0x0000
		CNetGamePlayer* m_LocalNetPlayer; //0x00E8
		char pad_00F0[144]; //0x00F0
		CNetGamePlayer* m_NetPlayers[32]; //0x0180
		uint16_t m_PlayerLimit; //0x0280
		char pad_0282[10]; //0x0282
		uint16_t m_PlayerCount; //0x028C
	}; //Size: 0x0290
	static_assert(sizeof(CNetworkPlayerMgr) == 0x290);
}
