#include "pch.h"
#include "Network.h"
#include "Rage/natives.h"
#include "Memory/Pointers.h"
#include "Script/PlayerInfo.h"
#include "Script/Network.h"

static Player s_SelectedPlayer = 0;
static void RenderPlayerList()
{
	for (Player i = 0; i < 32; i++)
	{
		const std::string PlayerName = (std::string{ Script::GetPlayerName(i) } + "##") + std::to_string(i);
		if (ImGui::Selectable(PlayerName.c_str(), s_SelectedPlayer == i))
			s_SelectedPlayer = i;
	}
}

static void RenderGeneralInfo()
{
	const rage::CNetworkPlayerMgr* PlayerManager = *Pointers::NetworkPlayerMgr;

	ImGui::Text("Is Session Started: %s", Script::IsSessionStarted() ? "true" : "false");

	if (PlayerManager)
	{
		ImGui::Text("CNetworkPlayerMgr: 0x%llX", reinterpret_cast<uintptr_t>(PlayerManager));
		ImGui::Text("Player Count: %u", PlayerManager->m_PlayerCount);
		ImGui::Text("Player Limit: %u", PlayerManager->m_PlayerLimit);
	}
}

static void RenderPlayerInfo()
{
	const std::string PlayerName = Script::GetPlayerName(s_SelectedPlayer);
	ImGui::Text("Name: %s", PlayerName.c_str());
	ImGui::Text("Index: %d", s_SelectedPlayer);
	ImGui::Text("netPlayerData: 0x%llX", Script::GetNetPlayerData(s_SelectedPlayer));
}

static void RenderNetworkMain()
{
	ImGui::BeginChild("player_list", ImVec2(200, 0));

	TRY
	{
		RenderPlayerList();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild(); // player_list
	ImGui::SameLine();
	ImGui::BeginChild("player_info");

	TRY
	{
		RenderGeneralInfo();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::Separator();

	TRY
	{
		RenderPlayerInfo();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild(); // player_info
}

void Menu::RenderNetworkTab()
{
	if (!ImGui::BeginTabItem("Network"))
		return;

	ImGui::BeginChild("network_child");

	TRY
	{
		RenderNetworkMain();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild(); // network_child
	ImGui::EndTabItem();
}
