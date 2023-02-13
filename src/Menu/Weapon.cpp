#include "pch.h"
#include "Weapon.h"
#include "Features.h"
#include "JobQueue.h"
#include "PlayerInfo.h"
#include "Lists.h"

namespace Menu
{
	void RenderWeaponTab()
	{
		if (!ImGui::BeginTabItem("Weapon"))
			return;

		ImGui::BeginChild("weapon_child");

		ImGui::BeginGroup();
		if (ImGui::Button("Give Weapons"))
			Features::GiveAllWeapons();

		if (ImGui::Button("Give Dual Wield Weapons"))
			Features::GiveAllDualWieldWeapons();

		if (ImGui::Button("Give Ammo"))
			Features::GiveAllAmmo();
			
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		if (ImGui::Button("Spawn Turret"))
		{
			QUEUE_JOB(=)
			{
				Features::SpawnVehicle(GATLING_GUN);
			}
			END_JOB()
		}

		if (ImGui::Button("Drop Current Weapon"))
			Features::DropCurrentWeapon();

		if (ImGui::Button("Remove All Weapons"))
		{
			QUEUE_JOB(=)
			{
				WEAPON::REMOVE_ALL_PED_WEAPONS(g_LocalPlayer.m_Entity, true, true);
			}
			END_JOB()
		}

		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::Checkbox("Infinite Ammo", g_Settings["infinite_ammo"].get<bool*>());
			
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("RGB Electric Lantern", g_Settings["rgb_elec_lantern"].get<bool*>());

		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		ImGui::Text("Weapon List");
		ImGui::BeginChild("weapon_list_menu", ImVec2(270, 0));

		static Hash CurrentWeapon = WEAPON_REVOLVER_DOUBLEACTION_EXOTIC;
		for (const auto& w : g_WeaponList)
		{
			if (ImGui::Selectable(w.first.c_str(), CurrentWeapon == w.second))
				CurrentWeapon = w.second;
		}
			
		ImGui::EndChild();
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Text("Weapon Name: %s", HUD::GET_STRING_FROM_HASH_KEY(CurrentWeapon));
		ImGui::Text("Weapon Hash: %X", CurrentWeapon);

		ImGui::Separator();
		if (ImGui::Button("Give Weapon"))
			Features::GiveWeapon(CurrentWeapon);
		ImGui::SameLine();
		if (ImGui::Button("Remove Weapon"))
			Features::RemoveWeapon(CurrentWeapon);

		ImGui::EndGroup();
		ImGui::EndChild();

		ImGui::EndTabItem();
	}
}
