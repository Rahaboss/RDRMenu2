#include "Menu.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Features.h"
#include "Thread/Thread.h"
#include "Script/World.h"
#include "Renderer/Renderer.h"
#include "Config/Settings.h"
#include "Config/Lists.h"
#include "Script/Player.h"
#include "Script/Ped.h"
#include "Script/Entity.h"
#include "PlayerInfo.h"
#include "Script/Spawning.h"
#include "Renderer/RGB.h"
#include "Script/Rendering.h"
#include "ESP/ESP.h"
#include "Rage/ScriptGlobal.h"

void Menu::RenderMenu()
{
	TRY
	{
		ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;
		Renderer::RGBTick();
		ESP::RenderPedBoneESP(ScriptGlobal(35).Get<Ped>());

		if (IsOpen)
		{
			if (ImGui::Begin("RDRMenu2", &IsOpen))
			{
				ImGui::BeginTabBar("tab_bar");
				RenderPlayerTab();
				RenderWeaponTab();
				RenderWorldTab();
				RenderTeleportTab();
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

	ImGui::ColorButton("RGB", Renderer::GetImGuiRGB());

	ImGui::EndChild();
	ImGui::EndTabItem();
}
