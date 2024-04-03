#include "pch.h"
#include "Network.h"
#include "Memory/Pointers.h"
#include "Script/PlayerInfo.h"

bool Script::IsSessionStarted()
{
	return *Pointers::IsSessionStarted;
}

rage::CNetGamePlayer* Script::GetNetGamePlayer(Player p)
{
	TRY
	{
		if (const rage::CNetworkPlayerMgr* PlayerMgr = *Pointers::NetworkPlayerMgr)
			return PlayerMgr->m_NetPlayers[p];
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return nullptr;
}

rage::netPlayerData* Script::GetNetPlayerData(Player p)
{
	TRY
	{
		if (rage::CNetGamePlayer* NetGamePlayer = GetNetGamePlayer(p))
		{
			return NetGamePlayer->GetNetPlayerData();
		}
		else if (!IsSessionStarted() && p == g_LocalPlayer.m_Index)
		{
			return reinterpret_cast<rage::netPlayerData*>(Pointers::GetOfflineNetPlayerData() + 0x20);
		}
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return nullptr;
}

const char* Script::GetPlayerName(Player p)
{
	if (rage::netPlayerData* PlayerData = GetNetPlayerData(p))
		return PlayerData->m_Name;

	return "N/A";
}
