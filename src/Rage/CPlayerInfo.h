#pragma once

#include "pch.h"
#include "CPed.h"
#include "CNetGamePlayer.h"

namespace rage
{
	class CPlayerInfo
	{
	public:
		char pad_0000[336]; //0x0000
		class netPlayerData m_NetPlayerData; //0x0150
		char pad_0220[4008]; //0x0220
		class CPed* m_Ped; //0x11C8
	}; //Size: 0x11D0
	static_assert(sizeof(CPlayerInfo) == 0x11D0);
}
