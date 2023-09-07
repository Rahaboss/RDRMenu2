#include "pch.h"
#include "Player.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/PlayerInfo.h"
#include "Script/Ped.h"
#include "Rage/natives.h"
#include "Script/Entity.h"

void Menu::RenderPlayerTab()
{
	if (!ImGui::BeginTabItem("Player"))
		return;

	ImGui::BeginChild("player_child");

	ImGui::SeparatorText("Buttons");
	if (ImGui::Button("Fill Cores"))
	{
		QUEUE_JOB(=)
		{
			Script::FillCores(g_LocalPlayer.m_Entity);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Give Gold Cores"))
	{
		QUEUE_JOB(=)
		{
			Script::GiveGoldCores(g_LocalPlayer.m_Entity);
		}
		END_JOB()
	}

	if (ImGui::Button("Explode Head"))
	{
		QUEUE_JOB(=)
		{
			PED::EXPLODE_PED_HEAD(g_LocalPlayer.m_Entity, RAGE_JOAAT("WEAPON_SNIPERRIFLE_CARCANO"));
		}
		END_JOB()
	}

	ImGui::SeparatorText("Toggles");
	if (ImGui::Checkbox("Godmode", g_Settings["player_godmode"].get<bool*>()))
	{
		if (!g_Settings["player_godmode"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Script::SetInvincible(g_LocalPlayer.m_Entity, false);
			}
			END_JOB()
		}
	}

	ImGui::EndChild();
	ImGui::EndTabItem();
}
