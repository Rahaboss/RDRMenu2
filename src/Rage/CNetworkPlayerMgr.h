#pragma once

#include "pch.h"
#include "CNetGamePlayer.h"

class CNetworkPlayerMgr
{
public:
	virtual ~CNetworkPlayerMgr() = 0; //0x0000
	char pad_0000[224]; //0x0000
	class CNetGamePlayer* m_LocalNetPlayer; //0x00E8
	char pad_00F0[144]; //0x00F0
	class CNetGamePlayer* m_NetPlayers[32]; //0x0180
	uint16_t m_PlayerLimit; //0x0280
	char pad_0282[10]; //0x0282
	uint16_t m_PlayerCount; //0x028C
}; //Size: 0x0290
static_assert(sizeof(CNetworkPlayerMgr) == 0x290);
