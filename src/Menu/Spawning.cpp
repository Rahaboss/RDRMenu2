#include "pch.h"
#include "Spawning.h"
#include "Rage/natives.h"
#include "Config/Lists.h"
#include "Thread/JobQueue.h"
#include "Script/Spawning.h"

void Menu::RenderVehicleSpawner()
{
	static Hash s_SelectedVehicle = RAGE_JOAAT("CART01");
	static bool s_WarpInside = true, s_AutoRemove = true;
	static Vehicle s_LastSpawnedVehicle = 0;
	
	ImGui::Text("Selected Vehicle: %s", Lists::GetHashName(s_SelectedVehicle).c_str());

	ImGui::Checkbox("Warp Inside Vehicle", &s_WarpInside);
	ImGui::SameLine();
	ImGui::Checkbox("Remove Automatically", &s_AutoRemove);
	
	if (ImGui::Button("Spawn Vehicle"))
	{
		QUEUE_JOB(=)
		{
			s_LastSpawnedVehicle = Script::SpawnVehicle(s_SelectedVehicle, s_WarpInside);

			if (s_AutoRemove)
				Script::SetEntityAsNoLongerNeeded(s_LastSpawnedVehicle);
		}
		END_JOB()
	}
	ImGui::SameLine();

	if (!s_LastSpawnedVehicle)
		ImGui::BeginDisabled();
	if (ImGui::Button("Remove Last Spawned Vehicle"))
	{
		QUEUE_JOB(=)
		{
			Script::DeleteEntity(s_LastSpawnedVehicle);
			s_LastSpawnedVehicle = 0;
		}
		END_JOB()
	}
	if (!s_LastSpawnedVehicle)
		ImGui::EndDisabled();

	ImGui::Separator();

	ImGui::TextUnformatted("Select Vehicle");
	ImGui::BeginChild("vehicle_child");

	for (const auto& [Name, Model] : Lists::VehicleList)
	{
		if (ImGui::Selectable(Name.c_str(), Model == s_SelectedVehicle))
			s_SelectedVehicle = Model;
	}

	ImGui::EndChild();
}

void Menu::RenderPedSpawner()
{
}

void Menu::RenderObjectSpawner()
{
}

void Menu::RenderSpawningTab()
{
	if (!ImGui::BeginTabItem("Spawning"))
		return;

	ImGui::BeginChild("spawning_child");

	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted("Select List:");
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

#if 0
	static int s_SelectedList = 0;
	const char* ComboLabels[]{
		"Objects",
		"Peds",
		"Vehicles",
	};

	ImGui::AlignTextToFramePadding();
	ImGui::Text("Select List");
	ImGui::SameLine();

	ImGui::SetNextItemWidth(100.0f);
	if (ImGui::BeginCombo("##list_select", ComboLabels[s_SelectedList]))
	{
		for (int i = 0; i < IM_ARRAYSIZE(ComboLabels); i++)
		{
			if (ImGui::Selectable(ComboLabels[i], i == s_SelectedList))
				s_SelectedList = i;
		}

		ImGui::EndCombo();
	}

	if (s_SelectedList == 0)
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
	else if (s_SelectedList == 1)
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
	else if (s_SelectedList == 2)
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
#endif

	ImGui::EndChild();
	ImGui::EndTabItem();
}
