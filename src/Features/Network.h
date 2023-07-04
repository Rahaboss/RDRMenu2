#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/CNetGamePlayer.h"
#include "Rage/CPlayerInfo.h"
#include "Rage/CNetworkPlayerMgr.h"

namespace Features
{
	bool IsSessionStarted();
	CNetGamePlayer* GetNetPlayer(Player player);
	CPlayerInfo* GetNetPlayerInfo(Player player);
	CPed* GetNetPlayerPed(Player player);
	CNetworkPlayerMgr* GetNetworkPlayerMgr();
	std::vector<Player> GetNetPlayerList();
}
