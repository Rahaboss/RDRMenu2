#pragma once

#include "pch.h"
#include "Rage/natives.h"
#include "Rage/CNetGamePlayer.h"

namespace Script
{
	bool IsSessionStarted();
	rage::CNetGamePlayer* GetNetGamePlayer(Player p);
	rage::CPlayerInfo* GetPlayerInfo(Player p);
	rage::netPlayerData* GetNetPlayerData(Player p);
	rage::CPed* GetPlayerPed(Player p);
	const char* GetPlayerName(Player p);
	void CopyIP(rage::netAddress IP);
	rage::CPlayerInfo* GetOfflinePlayerInfo();
}
