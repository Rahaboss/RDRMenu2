#include "pch.h"
#include "Network.h"
#include "Rage/natives.h"
#include "Memory/Pointers.h"
#include "Script/PlayerInfo.h"
#include "Script/Network.h"
#include "Config/Lists.h"
#include "Script/Entity.h"

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

static void RenderLocalPlayerInfo()
{
	const Hash PlayerModel = g_LocalPlayer.m_Model;
	ImGui::Text("Model: %s", Lists::GetHashNameOrUint(PlayerModel).c_str());

	if (const Entity MountEnt = g_LocalPlayer.m_Mount)
	{
		const Hash MountModel = Script::GetEntityModel(MountEnt);
		ImGui::Text("Mount: %s", Lists::GetHashNameOrUint(MountModel).c_str());
	}
	else
	{
		ImGui::BeginDisabled();
		ImGui::Text("Mount: N/A");
		ImGui::EndDisabled();
	}

	if (const Vehicle VehicleEnt = g_LocalPlayer.m_Vehicle)
	{
		const Hash VehicleModel = Script::GetEntityModel(VehicleEnt);
		ImGui::Text("Vehicle: %s", Lists::GetHashNameOrUint(VehicleModel).c_str());
	}
	else
	{
		ImGui::BeginDisabled();
		ImGui::Text("Vehicle: N/A");
		ImGui::EndDisabled();
	}
}

static void CopyIP(rage::netAddress Address)
{
	std::stringstream ss;
	ss << static_cast<uint32_t>(Address.m_Field1) << '.' << static_cast<uint32_t>(Address.m_Field2) << '.' <<
		static_cast<uint32_t>(Address.m_Field3) << '.' << static_cast<uint32_t>(Address.m_Field4);
	ImGui::SetClipboardText(ss.str().c_str());
}

static void RenderPlayerInfo()
{
	const std::string PlayerName = Script::GetPlayerName(s_SelectedPlayer);
	ImGui::Text("Name: %s", PlayerName.c_str());
	ImGui::Text("Index: %d", s_SelectedPlayer);

	const rage::netPlayerData* NetPlayerData = Script::GetNetPlayerData(s_SelectedPlayer);
	ImGui::Text("netPlayerData: 0x%llX", NetPlayerData);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##netPlayerData"))
	{
		std::stringstream ss;
		ss << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(NetPlayerData);
		ImGui::SetClipboardText(ss.str().c_str());
	}

	const rage::CNetGamePlayer* NetGamePlayer = Script::GetNetGamePlayer(s_SelectedPlayer);
	ImGui::Text("CNetGamePlayer: 0x%llX", NetGamePlayer);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##CNetGamePlayer"))
	{
		std::stringstream ss;
		ss << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(NetGamePlayer);
		ImGui::SetClipboardText(ss.str().c_str());
	}

	if (NetPlayerData)
	{
		ImGui::Text("Rockstar ID: %llu", NetPlayerData->m_RockstarID);

		ImGui::Text("Relay IP: %u.%u.%u.%u:%u",
			NetPlayerData->m_RelayIP.m_Field1,
			NetPlayerData->m_RelayIP.m_Field2,
			NetPlayerData->m_RelayIP.m_Field3,
			NetPlayerData->m_RelayIP.m_Field4,
			NetPlayerData->m_RelayPort);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Relay IP"))
			CopyIP(NetPlayerData->m_RelayIP);

		ImGui::Text("External IP: %u.%u.%u.%u:%u",
			NetPlayerData->m_ExternalIP.m_Field1,
			NetPlayerData->m_ExternalIP.m_Field2,
			NetPlayerData->m_ExternalIP.m_Field3,
			NetPlayerData->m_ExternalIP.m_Field4,
			NetPlayerData->m_ExternalPort);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##External IP"))
			CopyIP(NetPlayerData->m_ExternalIP);

		ImGui::Text("Internal IP: %u.%u.%u.%u:%u",
			NetPlayerData->m_InternalIP.m_Field1,
			NetPlayerData->m_InternalIP.m_Field2,
			NetPlayerData->m_InternalIP.m_Field3,
			NetPlayerData->m_InternalIP.m_Field4,
			NetPlayerData->m_InternalPort);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Internal IP"))
			CopyIP(NetPlayerData->m_InternalIP);
	}
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

	ImGui::Separator();

	TRY
	{
		RenderLocalPlayerInfo();
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
