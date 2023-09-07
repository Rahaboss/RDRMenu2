#include "pch.h"
#include "Weapon.h"
#include "Config/Settings.h"
#include "Config/Lists.h"
#include "Script/Weapon.h"
#include "Script/PlayerInfo.h"
#include "Thread/JobQueue.h"

void Menu::RenderWeaponTab()
{
	if (!ImGui::BeginTabItem("Weapon"))
		return;

	ImGui::BeginChild("weapon_child");

	ImGui::BeginChild("##left_half", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));

	ImGui::Checkbox("Infinite Ammo", g_Settings["infinite_ammo"].get<bool*>());
	ImGui::Checkbox("Rapid Fire", g_Settings["rapid_fire"].get<bool*>());

	ImGui::EndChild();
	ImGui::SameLine();
	ImGui::BeginChild("##right_half");

	for (const auto& w : Lists::WeaponList)
	{
		if (ImGui::Selectable(w.first.c_str()))
		{
			QUEUE_JOB(=)
			{
				Script::GiveWeapon(g_LocalPlayer.m_Entity, w.second);
			}
			END_JOB()
		}
	}
	
	ImGui::EndChild();

	ImGui::EndChild();
	ImGui::EndTabItem();
}
