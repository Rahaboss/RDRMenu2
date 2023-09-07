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
