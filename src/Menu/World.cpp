#include "pch.h"
#include "World.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/World.h"

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
