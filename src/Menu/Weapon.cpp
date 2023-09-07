#include "pch.h"
#include "Weapon.h"
#include "Config/Settings.h"

void Menu::RenderWeaponTab()
{
	if (!ImGui::BeginTabItem("Weapon"))
		return;

	ImGui::BeginChild("weapon_child");

	ImGui::Checkbox("Infinite Ammo", g_Settings["infinite_ammo"].get<bool*>());
	ImGui::Checkbox("Rapid Fire", g_Settings["rapid_fire"].get<bool*>());

	ImGui::EndChild();
	ImGui::EndTabItem();
}
