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
		if (IsSessionStarted() && p >= 0 && p < 32)
		{
			if (const rage::CNetworkPlayerMgr* PlayerMgr = *Pointers::NetworkPlayerMgr)
				return PlayerMgr->m_NetPlayers[p];
		}
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return nullptr;
}

rage::CPlayerInfo* Script::GetPlayerInfo(Player p)
{
	if (IsSessionStarted() && (p >= 0 && p < 32))
	{
		if (rage::CNetGamePlayer* NetGamePlayer = GetNetGamePlayer(p))
			return NetGamePlayer->m_PlayerInfo;
	}
	else if (p == g_LocalPlayer.m_Index)
		return GetOfflinePlayerInfo();

	return nullptr;
}

rage::netPlayerData* Script::GetNetPlayerData(Player p)
{
	TRY
	{
		if (IsSessionStarted() && (p >= 0 && p < 32))
		{
			if (rage::CNetGamePlayer* NetGamePlayer = GetNetGamePlayer(p))
				return NetGamePlayer->GetNetPlayerData();
		}
		else if (p == g_LocalPlayer.m_Index)
		{
			if (rage::CPlayerInfo* PlayerInfo = GetOfflinePlayerInfo())
				return &PlayerInfo->m_NetPlayerData;
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

void Script::CopyIP(rage::netAddress IP)
{
	std::stringstream ss;
	ss << static_cast<uint32_t>(IP.m_Field1) << '.' << static_cast<uint32_t>(IP.m_Field2) << '.' <<
		static_cast<uint32_t>(IP.m_Field3) << '.' << static_cast<uint32_t>(IP.m_Field4);
	ImGui::SetClipboardText(ss.str().c_str());
}

rage::CPlayerInfo* Script::GetOfflinePlayerInfo()
{
	if (!IsSessionStarted() && g_LocalPlayer.m_Ped)
	{
		const uint32_t x = g_LocalPlayer.m_Ped->m_9C & 0x1FFFF;
		return reinterpret_cast<rage::CPlayerInfo*>(*reinterpret_cast<uint64_t*>(0x148 * x + *Pointers::qword_7FF66EEBCE48 + 0xF0) & ~1);
	}

	return nullptr;
}
