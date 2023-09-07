#include "Menu.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Script/Features.h"
#include "Thread/Thread.h"
#include "Script/World.h"
#include "Renderer/Renderer.h"
#include "Config/Settings.h"
#include "Config/Lists.h"
#include "Script/Player.h"
#include "Script/Ped.h"
#include "Script/Entity.h"
#include "Script/PlayerInfo.h"
#include "Script/Spawning.h"
#include "Renderer/RGB.h"
#include "Script/Rendering.h"
#include "Rage/ScriptGlobal.h"
#include "Util/Timer.h"
#include "Rendering.h"

void Menu::RenderMenu()
{
	TRY
	{
		ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;
		Renderer::RGBTick();
		ImVec4 title_bg = Renderer::GetImGuiRGBA(); title_bg.x *= 0.6f; title_bg.y *= 0.6f; title_bg.z *= 0.6f;
		ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = title_bg;

		if (IsOpen)
		{
			if (ImGui::Begin("RDRMenu2", &IsOpen, ImGuiWindowFlags_NoCollapse))
			{
				ImGui::BeginTabBar("tab_bar");
				RenderPlayerTab();
				RenderWeaponTab();
				RenderWorldTab();
				RenderTeleportTab();
				RenderSpawningTab();
				RenderRenderingMenu();
				RenderDebugTab();
				if (ImGui::BeginTabItem("Exit"))
				{
					Features::StartExit();
					ImGui::EndTabItem();
				}
				ImGui::EndTabBar();
			}
			ImGui::End();
		}
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

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

void Menu::RenderWorldTab()
{
	if (!ImGui::BeginTabItem("World"))
		return;

	ImGui::BeginChild("world_child");

	if (ImGui::Button("Noon + Sunny"))
		JobQueue::Add(Script::NoonAndSunny);

	ImGui::Checkbox("Disable West Elizabeth Pinkerton Patrols", g_Settings["disable_pinkerton_patrols"].get<bool*>());
	ImGui::Checkbox("Enable All DLCs", g_Settings["enable_dlcs"].get<bool*>());

	ImGui::EndChild();
	ImGui::EndTabItem();
}

void Menu::RenderTeleportTab()
{
	if (!ImGui::BeginTabItem("Teleport"))
		return;

	ImGui::BeginChild("teleport_child");

	for (size_t i = 0; i < Lists::TeleportList.size(); i++)
	{
		if (ImGui::Selectable(Lists::TeleportList[i].first.c_str()))
		{
			QUEUE_JOB(=)
			{
				Script::TeleportOnGround(Lists::TeleportList[i].second);
			}
			END_JOB()
		}
	}

	ImGui::EndChild();
	ImGui::EndTabItem();
}

void Menu::RenderSpawningTab()
{
	if (!ImGui::BeginTabItem("Spawning"))
		return;

	ImGui::BeginChild("spawning_child");

	static int s_SeletectedList = 0;
	const char* ComboLabels[]{
		"Objects",
		"Peds",
		"Vehicles",
	};

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Select List");
	ImGui::SameLine();

	ImGui::SetNextItemWidth(100.0f);
	if (ImGui::BeginCombo("##list_select", ComboLabels[s_SeletectedList]))
	{
		for (int i = 0; i < IM_ARRAYSIZE(ComboLabels); i++)
		{
			if (ImGui::Selectable(ComboLabels[i], i == s_SeletectedList))
				s_SeletectedList = i;
		}

		ImGui::EndCombo();
	}

	if (s_SeletectedList == 0)
	{
		if (ImGui::BeginChild("##object_list"))
		{
			for (const auto& [Name, Model] : Lists::ObjectList)
			{
				if (ImGui::Selectable(Name.c_str()))
				{
					Hash m = Model;
					QUEUE_JOB(=)
					{
						Script::SpawnObject(m);
					}
					END_JOB()
				}
			}
		}
		ImGui::EndChild();
	}
	else if (s_SeletectedList == 1)
	{
		if (ImGui::BeginChild("##ped_list"))
		{
			for (const auto& [Name, Model] : Lists::PedList)
			{
				if (ImGui::Selectable(Name.c_str()))
				{
					Hash m = Model;
					QUEUE_JOB(=)
					{
						Script::SpawnPed(m);
					}
					END_JOB()
				}
			}
		}
		ImGui::EndChild();
	}
	else if (s_SeletectedList == 2)
	{
		if (ImGui::BeginChild("##vehicle_list"))
		{
			for (const auto& [Name, Model] : Lists::VehicleList)
			{
				if (ImGui::Selectable(Name.c_str()))
				{
					Hash m = Model;
					QUEUE_JOB(=)
					{
						Script::SpawnVehicle(m);
					}
					END_JOB()
				}
			}
		}
		ImGui::EndChild();
	}

	ImGui::EndChild();
	ImGui::EndTabItem();
}

void Menu::RenderDebugTab()
{
	if (!ImGui::BeginTabItem("Debug"))
		return;

	ImGui::BeginChild("debug_child");

	if (ImGui::CollapsingHeader("Settings JSON"))
	{
		ImGui::PushFont(Renderer::DefaultFont);

		ImGui::Text(g_Settings.dump(4).c_str());

		ImGui::PopFont();
	}

	ImGui::SeparatorText("Buttons");
	if (ImGui::Button("Spawn Ped"))
	{
		QUEUE_JOB(=)
		{
			Script::SpawnPed(RAGE_JOAAT("CS_DUTCH"));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Spawn Vehicle"))
	{
		QUEUE_JOB(=)
		{
			Script::SpawnVehicle(RAGE_JOAAT("WAGON02X"));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Spawn Object"))
	{
		QUEUE_JOB(=)
		{
			Script::SpawnObject(RAGE_JOAAT("P_STOOL01X"));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Spawn Pickup"))
	{
		QUEUE_JOB(=)
		{
			Script::SpawnPickup(RAGE_JOAAT("PICKUP_WEAPON_PISTOL_M1899"));
		}
		END_JOB()
	}

	if (ImGui::Button("Change Player Model"))
	{
		QUEUE_JOB(=)
		{
			Script::SetPlayerModel(RAGE_JOAAT("CS_DUTCH"));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Reset Player Model"))
	{
		JobQueue::Add(Script::ResetPlayerModel);
	}

	ImGui::ColorButton("RGB", Renderer::GetImGuiRGBA());

	if (ImGui::CollapsingHeader("Object List"))
	{
		ImGui::Text("Total Objects: %u", Lists::ObjectList.size());
		if (ImGui::BeginChild("##object_list"))
		{
			for (const auto& [Name, Model] : Lists::ObjectList)
				ImGui::Text(Name.c_str());
		}
		ImGui::EndChild();
	}
	if (ImGui::CollapsingHeader("Ped List"))
	{
		ImGui::Text("Total Peds: %u", Lists::PedList.size());
		if (ImGui::BeginChild("##ped_list"))
		{
			for (const auto& [Name, Model] : Lists::PedList)
				ImGui::Text(Name.c_str());
		}
		ImGui::EndChild();
	}
	if (ImGui::CollapsingHeader("Weapon List"))
	{
		ImGui::Text("Total Weapons: %u", Lists::WeaponList.size());
		if (ImGui::BeginChild("##weapon_list"))
		{
			for (const auto& [Name, Model] : Lists::WeaponList)
				ImGui::Text(Name.c_str());
		}
		ImGui::EndChild();
	}
	if (ImGui::CollapsingHeader("Vehicle List"))
	{
		ImGui::Text("Total Vehicles: %u", Lists::VehicleList.size());
		if (ImGui::BeginChild("##vehicle_list"))
		{
			for (const auto& [Name, Model] : Lists::VehicleList)
				ImGui::Text(Name.c_str());
		}
		ImGui::EndChild();
	}

	ImGui::Text("ESP Time: %.3fms", Timer::s_ESPTime);
	ImGui::Text("Script Thread Time: %.3fms", Timer::s_ScriptThreadTime);
	ImGui::Text("Job Queue Time: %.3fms", Timer::s_JobQueueTime);

	ImGui::EndChild();
	ImGui::EndTabItem();
}
