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
		if (rage::CNetworkPlayerMgr* PlayerMgr = *Pointers::NetworkPlayerMgr)
		{
			if (rage::CNetGamePlayer* NetGamePlayer = PlayerMgr->m_NetPlayers[p])
			{
				if (NetGamePlayer->IsValid())
					return NetGamePlayer;
			}
		}
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
			return Pointers::GetOfflineNetPlayerData();
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
