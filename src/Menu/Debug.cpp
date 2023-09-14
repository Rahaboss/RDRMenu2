#include "pch.h"
#include "Debug.h"
#include "Renderer/Renderer.h"
#include "Renderer/RGB.h"
#include "Config/Settings.h"
#include "Config/Lists.h"
#include "Thread/JobQueue.h"
#include "Script/Spawning.h"
#include "Script/Player.h"
#include "Util/Timer.h"
#include "Script/Ped.h"
#include "Script/PlayerInfo.h"

void Menu::RenderDebugTab()
{
	if (!ImGui::BeginTabItem("Debug"))
		return;

	ImGui::BeginChild("debug_child");

	if (ImGui::CollapsingHeader("Settings JSON"))
	{
		ImGui::PushFont(Renderer::DefaultFont);

		ImGui::Text(Lists::MetaPedOutfits.dump(4).c_str());

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

	if (ImGui::Button("Reload Settings"))
		JobQueue::Add(Settings::Create);
	ImGui::SameLine();
	if (ImGui::Button("Reload Lists"))
		JobQueue::Add(Lists::Create);
	ImGui::SameLine();
	if (ImGui::Button("Test"))
	{
		QUEUE_JOB(=)
		{
			Script::SetPlayerModel(RAGE_JOAAT("MSP_SALOON1_FEMALES_01"));
			Script::SetMetaPedOutfit(g_LocalPlayer.m_Entity, 1467286073);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Test2"))
	{
		QUEUE_JOB(=)
		{
			Script::SetPlayerModel(RAGE_JOAAT("MSP_SALOON1_MALES_01"));
			Script::SetMetaPedOutfit(g_LocalPlayer.m_Entity, 1467286073);
		}
		END_JOB()
	}

	ImGui::SeparatorText("Toggles");
	ImGui::Checkbox("Render ImGui Demo", g_Settings["render_imgui_demo"].get<bool*>());

	ImGui::SeparatorText("Benchmark Times");
	ImGui::Text("Render Thread: %.3fms", Timer::s_RenderThreadTime);
	ImGui::Text("\tESP: %.3fms", Timer::s_ESPTime);
	ImGui::Text("\tMenu: %.3fms", Timer::s_MenuTime);

	ImGui::Text("Script Thread Time: %.3fms", Timer::s_ScriptThreadTime);
	ImGui::Text("\tScript Thread Tick: %.3fms", Timer::s_ScriptThreadTickTime);
	ImGui::Text("\tJob Queue: %.3fms (%llu jobs)", Timer::s_JobQueueTime, JobQueue::GetJobsQueued());

	ImGui::Separator();
	ImGui::AlignTextToFramePadding();
	ImGui::Text("RGB");
	ImGui::SameLine();
	ImGui::ColorButton("##RGB", Renderer::GetImGuiRGBA());

	ImGui::EndChild();
	ImGui::EndTabItem();
}
