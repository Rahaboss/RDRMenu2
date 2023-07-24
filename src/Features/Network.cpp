#include "pch.h"
#include "Network.h"
#include "Features.h"
#include "Pointers.h"

namespace Features
{
	bool IsSessionStarted()
	{
		if (Pointers::IsSessionStarted)
			return *Pointers::IsSessionStarted;

		return false;
	}
	
	rage::CNetGamePlayer* GetNetPlayer(Player player)
	{
		if (Pointers::GetNetworkPlayer)
			return Pointers::GetNetworkPlayer(player);

		return nullptr;
	}
	
	rage::CPlayerInfo* GetNetPlayerInfo(Player player)
	{
		if (auto NetPlayer = GetNetPlayer(player))
			return NetPlayer->m_PlayerInfo;

		return nullptr;
	}
	
	rage::CPed* GetNetPlayerPed(Player player)
	{
		if (auto NetPlayerInfo = GetNetPlayerInfo(player))
			return NetPlayerInfo->m_Ped;

		return nullptr;
	}
	
	rage::CNetworkPlayerMgr* GetNetworkPlayerMgr()
	{
		if (auto NetPlayerMgr = Pointers::NetworkPlayerMgr)
			return *NetPlayerMgr;

		return nullptr;
	}
	
	std::vector<Player> GetNetPlayerList()
	{
		std::vector<Player> Result;
		if (auto NetPlayerMgr = GetNetworkPlayerMgr())
		{
			for (const auto NetPlayer : NetPlayerMgr->m_NetPlayers)
			{
				if (NetPlayer && NetPlayer->IsValid())
					Result.push_back(static_cast<Player>(NetPlayer->m_PlayerID));
			}
		}

		return Result;
	}
}
