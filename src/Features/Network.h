#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/CNetGamePlayer.h"
#include "Rage/CPlayerInfo.h"
#include "Rage/CNetworkPlayerMgr.h"

namespace Features
{
	bool IsSessionStarted();
	rage::CNetGamePlayer* GetNetPlayer(Player player);
	rage::CPlayerInfo* GetNetPlayerInfo(Player player);
	rage::CPed* GetNetPlayerPed(Player player);
	rage::CNetworkPlayerMgr* GetNetworkPlayerMgr();
	std::vector<Player> GetNetPlayerList();
}
