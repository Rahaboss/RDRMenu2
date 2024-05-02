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
		const char* NameString = Script::GetPlayerName(i);
		const std::string PlayerName = (std::string{ NameString ? NameString : "N/A" } + "##") + std::to_string(i);
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

static void CopyPointer(const void* Pointer)
{
	std::stringstream ss;
	ss << std::hex << std::uppercase << reinterpret_cast<uintptr_t>(Pointer);
	ImGui::SetClipboardText(ss.str().c_str());
}

static void RenderPlayerInfo()
{
	ImGui::Text("Index: %d", s_SelectedPlayer);

	const rage::CNetGamePlayer* NetGamePlayer = Script::GetNetGamePlayer(s_SelectedPlayer);
	ImGui::Text("CNetworkPlayerMgr: 0x%llX (RDR2.exe+%llX)", (uintptr_t)Pointers::NetworkPlayerMgr,
		(uintptr_t)Pointers::NetworkPlayerMgr - g_BaseAddress);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##CNetGamePlayer"))
		CopyPointer(NetGamePlayer);

	const rage::CPlayerInfo* PlayerInfo = Script::GetPlayerInfo(s_SelectedPlayer);
	ImGui::Text("CPlayerInfo: 0x%llX", PlayerInfo);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##CPlayerInfo"))
		CopyPointer(PlayerInfo);

	const rage::netPlayerData* NetPlayerData = Script::GetNetPlayerData(s_SelectedPlayer);
	ImGui::Text("netPlayerData: 0x%llX", NetPlayerData);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##netPlayerData"))
		CopyPointer(NetPlayerData);

	if (NetPlayerData)
	{
		const char* PlayerName = NetPlayerData->m_Name;
		ImGui::Text("\tName: %s", PlayerName);

		ImGui::Text("\tRockstar ID: %llu", NetPlayerData->m_RockstarID);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Rockstar ID"))
		{
			std::string RockstarID = std::to_string(NetPlayerData->m_RockstarID);
			ImGui::SetClipboardText(RockstarID.c_str());
		}

		ImGui::Text("\tRelay IP: %u.%u.%u.%u:%u",
			NetPlayerData->m_RelayIP.m_Field1,
			NetPlayerData->m_RelayIP.m_Field2,
			NetPlayerData->m_RelayIP.m_Field3,
			NetPlayerData->m_RelayIP.m_Field4,
			NetPlayerData->m_RelayPort);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Relay IP"))
			Script::CopyIP(NetPlayerData->m_RelayIP);

		ImGui::Text("\tExternal IP: %u.%u.%u.%u:%u",
			NetPlayerData->m_ExternalIP.m_Field1,
			NetPlayerData->m_ExternalIP.m_Field2,
			NetPlayerData->m_ExternalIP.m_Field3,
			NetPlayerData->m_ExternalIP.m_Field4,
			NetPlayerData->m_ExternalPort);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##External IP"))
			Script::CopyIP(NetPlayerData->m_ExternalIP);

		ImGui::Text("\tInternal IP: %u.%u.%u.%u:%u",
			NetPlayerData->m_InternalIP.m_Field1,
			NetPlayerData->m_InternalIP.m_Field2,
			NetPlayerData->m_InternalIP.m_Field3,
			NetPlayerData->m_InternalIP.m_Field4,
			NetPlayerData->m_InternalPort);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##Internal IP"))
			Script::CopyIP(NetPlayerData->m_InternalIP);
	}

	const rage::CPed* Ped = Script::GetPlayerPed(s_SelectedPlayer);
	ImGui::Text("CPed: 0x%llX", Ped);
	ImGui::SameLine();
	if (ImGui::SmallButton("Copy##CPed"))
		CopyPointer(Ped);

	if (Ped)
	{
		ImGui::Text("\tCPed+0x9C: %u (0x%X)", Ped->m_9C, Ped->m_9C);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##CPed+0x9C"))
		{
			std::string s = std::to_string(Ped->m_9C);
			ImGui::SetClipboardText(s.c_str());
		}

		ImGui::Text("\tCPed+0x9C (masked): %u (0x%X)", Ped->m_9C & 0x1FFFF, Ped->m_9C & 0x1FFFF);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##CPed+0x9C masked"))
		{
			std::string s = std::to_string(Ped->m_9C & 0x1FFFF);
			ImGui::SetClipboardText(s.c_str());
		}

		ImGui::Text("\tCPed+0xE0: 0x%X", Ped->m_E0);
		ImGui::SameLine();
		if (ImGui::SmallButton("Copy##CPed+0xE0"))
			CopyPointer(Ped->m_E0);
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
