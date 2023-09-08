#include "pch.h"
#include "Teleport.h"
#include "Config/Lists.h"
#include "Thread/JobQueue.h"
#include "Script/Player.h"

void Menu::RenderTeleportTab()
{
	if (!ImGui::BeginTabItem("Teleport"))
		return;

	ImGui::BeginChild("teleport_child");
	
	ImGui::SeparatorText("Buttons");
	if (ImGui::Button("Teleport To Waypoint"))
		JobQueue::Add(Script::TeleportToWaypoint);
	ImGui::SameLine();
	ImGui::PushButtonRepeat(true);
	if (ImGui::Button("Teleport Through Door"))
		JobQueue::Add(Script::TeleportThroughDoor);
	ImGui::PopButtonRepeat();

	ImGui::SeparatorText("Teleport To Location");
	ImGui::BeginChild("teleport_list");

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

	ImGui::EndChild();
	ImGui::EndTabItem();
}
