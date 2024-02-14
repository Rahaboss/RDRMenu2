#pragma once

#include "pch.h"
#include "Rage/natives.h"
#include "Rage/CNetGamePlayer.h"

namespace Script
{
	bool IsSessionStarted();
	rage::CNetGamePlayer* GetNetGamePlayer(Player p);
	rage::netPlayerData* GetNetPlayerData(Player p);
	const char* GetPlayerName(Player p);
}
