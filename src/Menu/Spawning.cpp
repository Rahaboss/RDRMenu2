#include "pch.h"
#include "Spawning.h"
#include "Rage/natives.h"
#include "Config/Lists.h"
#include "Thread/JobQueue.h"
#include "Script/Spawning.h"

void Menu::RenderSpawningTab()
{
	if (!ImGui::BeginTabItem("Spawning"))
		return;

	ImGui::BeginChild("spawning_child");

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

	ImGui::EndChild();
	ImGui::EndTabItem();
}
