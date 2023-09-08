#include "pch.h"
#include "Spawning.h"
#include "Rage/natives.h"
#include "Config/Lists.h"
#include "Thread/JobQueue.h"
#include "Script/Spawning.h"
#include "Script/PlayerInfo.h"
#include "Util/String.h"
#include "Thread/Thread.h"

static void RenderSpawnerList(Hash& OutModel, const char* Filter, const std::map<std::string, Hash>& List)
{
	std::string FilterUpper{ Filter };
	Util::StringToUpper(FilterUpper);

	for (const auto& [Name, Model] : List)
	{
		std::string NameUpper{ Name };
		Util::StringToUpper(NameUpper);

		if (NameUpper.find(FilterUpper) == std::string::npos)
			continue;

		if (ImGui::Selectable(Name.c_str(), Model == OutModel))
			OutModel = Model;
	}
}

void Menu::RenderVehicleSpawner()
{
	static Hash s_SelectedVehicle = RAGE_JOAAT("CART01");
	static bool s_WarpInside = true, s_AutoRemove = true, s_RemovePrevious = true;
	static Vehicle s_LastSpawnedVehicle = 0;
	static char s_VehicleFilter[200]{};

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Spawning Settings"))
	{
		ImGui::Checkbox("Warp Inside Vehicle", &s_WarpInside);
		ImGui::SameLine();
		ImGui::Checkbox("Remove Automatically", &s_AutoRemove);
		ImGui::SameLine();
		ImGui::Checkbox("Remove Previous", &s_RemovePrevious);

		ImGui::SetNextItemWidth(300.0f);
		ImGui::InputText("Filter Vehicles", s_VehicleFilter, IM_ARRAYSIZE(s_VehicleFilter));
		ImGui::SameLine();
		if (ImGui::Button("Clear Filter"))
			s_VehicleFilter[0] = '\0';
	}
	
	ImGui::SeparatorText("Select Vehicle");

	if (ImGui::Button("Spawn Vehicle"))
	{
		QUEUE_JOB(=)
		{
			if (s_RemovePrevious)
				Script::DeleteEntity(s_LastSpawnedVehicle);

			s_LastSpawnedVehicle = Script::SpawnVehicle(s_SelectedVehicle, s_WarpInside);

			if (s_AutoRemove)
				Script::SetEntityAsNoLongerNeeded(s_LastSpawnedVehicle);
		}
		END_JOB()
	}
	ImGui::SameLine();
	ImGui::Text("Selected Vehicle: %s", Lists::GetHashName(s_SelectedVehicle).c_str());

	ImGui::BeginChild("vehicle_child");

	RenderSpawnerList(s_SelectedVehicle, s_VehicleFilter, Lists::VehicleList);

	ImGui::EndChild();
}

void Menu::RenderPedSpawner()
{
	static Hash s_SelectedPed = RAGE_JOAAT("CS_DUTCH");
	static bool s_AutoRemove = true, s_SetOntoMount = true, s_RemovePrevious = true, s_SpawnDead = false;
	static Ped s_LastSpawnedPed = 0;
	static char s_PedFilter[200]{};
	static int s_SelectedPedList = 0;

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Spawning Settings"))
	{
		ImGui::RadioButton("Full List", &s_SelectedPedList, 0);
		ImGui::SameLine();
		ImGui::RadioButton("Preset List", &s_SelectedPedList, 1);

		ImGui::Checkbox("Remove Automatically", &s_AutoRemove);
		ImGui::SameLine();
		ImGui::Checkbox("Set Onto Mount", &s_SetOntoMount);
		ImGui::SameLine();
		ImGui::Checkbox("Remove Previous", &s_RemovePrevious);
		ImGui::SameLine();
		ImGui::Checkbox("Spawn Dead", &s_SpawnDead);

		ImGui::SetNextItemWidth(300.0f);
		ImGui::InputText("Filter Peds", s_PedFilter, IM_ARRAYSIZE(s_PedFilter));
		ImGui::SameLine();
		if (ImGui::Button("Clear Filter"))
			s_PedFilter[0] = '\0';
	}

	ImGui::SeparatorText("Select Ped");

	if (ImGui::Button("Spawn Ped"))
	{
		QUEUE_JOB(=)
		{
			if (s_RemovePrevious)
				Script::DeleteEntity(s_LastSpawnedPed);
			
			s_LastSpawnedPed = Script::SpawnPed(s_SelectedPed);

			if (s_SetOntoMount && PED::_IS_MOUNT_SEAT_FREE(s_LastSpawnedPed, -1))
				PED::SET_PED_ONTO_MOUNT(g_LocalPlayer.m_Entity, s_LastSpawnedPed, -1, true);

			if (s_SpawnDead)
				ENTITY::SET_ENTITY_HEALTH(s_LastSpawnedPed, 0, 0);

			if (s_AutoRemove)
				Script::SetEntityAsNoLongerNeeded(s_LastSpawnedPed);
		}
		END_JOB()
	}
	ImGui::SameLine();
	ImGui::Text("Selected Ped: %s", Lists::GetHashName(s_SelectedPed).c_str());

	ImGui::BeginChild("ped_child");

	if (s_SelectedPedList == 0)
		RenderSpawnerList(s_SelectedPed, s_PedFilter, Lists::PedList);
	else
		RenderSpawnerList(s_SelectedPed, s_PedFilter, Lists::PresetPedList);

	ImGui::EndChild();
}

void Menu::RenderObjectSpawner()
{
	static Hash s_SelectedObject = RAGE_JOAAT("S_UFO01X");
	static bool s_AutoRemove = true, s_RemovePrevious = true;
	static Object s_LastSpawnedObject = 0;
	static char s_ObjectFilter[200]{};

	ImGui::SetNextItemOpen(true, ImGuiCond_Once);
	if (ImGui::CollapsingHeader("Spawning Settings"))
	{
		ImGui::Checkbox("Remove Automatically", &s_AutoRemove);
		ImGui::SameLine();
		ImGui::Checkbox("Remove Previous", &s_RemovePrevious);

		ImGui::SetNextItemWidth(300.0f);
		ImGui::InputText("Filter Objects", s_ObjectFilter, IM_ARRAYSIZE(s_ObjectFilter));
		ImGui::SameLine();
		if (ImGui::Button("Clear Filter"))
			s_ObjectFilter[0] = '\0';
	}
	
	ImGui::SeparatorText("Select Object");

	if (ImGui::Button("Spawn Object"))
	{
		QUEUE_JOB(=)
		{
			if (s_RemovePrevious)
				Script::DeleteEntity(s_LastSpawnedObject);

			s_LastSpawnedObject = Script::SpawnObject(s_SelectedObject);

			if (s_AutoRemove)
				Script::SetEntityAsNoLongerNeeded(s_LastSpawnedObject);
		}
		END_JOB()
	}
	ImGui::SameLine();
	ImGui::Text("Selected Object: %s", Lists::GetHashName(s_SelectedObject).c_str());

	ImGui::BeginChild("object_child");

	RenderSpawnerList(s_SelectedObject, s_ObjectFilter, Lists::ObjectList);

	ImGui::EndChild();
}

void Menu::RenderSpawningTab()
{
	if (!ImGui::BeginTabItem("Spawning"))
		return;

	ImGui::BeginChild("spawning_child");

	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted("Select Spawner:");
	ImGui::SameLine();

	static int s_CurrentList = 0;
	ImGui::RadioButton("Vehicle", &s_CurrentList, 0);
	ImGui::SameLine();
	ImGui::RadioButton("Ped", &s_CurrentList, 1);
	ImGui::SameLine();
	ImGui::RadioButton("Object", &s_CurrentList, 2);

	ImGui::Separator();

	switch (s_CurrentList)
	{
	case 0:
		RenderVehicleSpawner();
		break;
	case 1:
		RenderPedSpawner();
		break;
	case 2:
		RenderObjectSpawner();
		break;
	}

	ImGui::EndChild();
	ImGui::EndTabItem();
}
