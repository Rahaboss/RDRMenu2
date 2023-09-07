#include "pch.h"
#include "Teleport.h"
#include "Config/Lists.h"
#include "Thread/JobQueue.h"
#include "Script/Player.h"

void Menu::RenderTeleportTab()
{
	if (!ImGui::BeginTabItem("Teleport"))
		return;

	ImGui::SeparatorText("Teleport To Location");
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
