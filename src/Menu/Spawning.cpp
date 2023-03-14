#include "pch.h"
#include "Spawning.h"
#include "Lists.h"
#include "JobQueue.h"
#include "Features.h"

namespace Menu
{
	void RenderSpawningTab()
	{
		if (!ImGui::BeginTabItem("Spawning"))
			return;
		
		ImGui::BeginChild("spawning_child");

		const float ChildHeight = ImGui::GetContentRegionAvail().y / 2;

		ImGui::BeginChild("ped_menu_outer", ImVec2(0, ChildHeight));
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Spawn Ped");
		ImGui::SameLine();
		static char PedBuffer[200];
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText("###filter_ped", PedBuffer, 200, ImGuiInputTextFlags_CharsUppercase);
		ImGui::BeginChild("ped_menu");
		for (const auto& it : g_PedList)
		{
			if (it.first.find(PedBuffer) == std::string::npos)
				continue;

			if (ImGui::Selectable(it.first.c_str()))
			{
				QUEUE_JOB(=)
				{
					Features::SpawnPed(it.second);
				}
				END_JOB()
			}
		}
		ImGui::EndChild();
		ImGui::EndChild();
		ImGui::Separator();

		ImGui::BeginChild("vehicle_menu_outer");
		ImGui::AlignTextToFramePadding();
		ImGui::Text("Spawn Vehicle");
		ImGui::SameLine();
		static char VehBuffer[200];
		ImGui::PushItemWidth(250.0f);
		ImGui::InputText("###filter_veh", VehBuffer, 200, ImGuiInputTextFlags_CharsUppercase);
		ImGui::BeginChild("vehicle_menu");
		for (const auto& it : g_VehicleList)
		{
			if (it.first.find(VehBuffer) == std::string::npos)
				continue;

			if (ImGui::Selectable(it.first.c_str()))
			{
				QUEUE_JOB(=)
				{
					Features::SpawnVehicle(it.second);
				}
				END_JOB()
			}
		}
		ImGui::EndChild();
		ImGui::EndChild();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
