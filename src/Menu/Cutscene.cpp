#include "pch.h"
#include "Cutscene.h"
#include "Config/Lists.h"

void Menu::RenderCutsceneTab()
{
	if (!ImGui::BeginTabItem("Cutscene"))
		return;

	ImGui::SeparatorText("Cutscenes");
	ImGui::BeginChild("cutscene_child");

	for (const json& Cutscene : Lists::CutsceneList)
	{
		ImGui::Selectable(Cutscene["id"].get_ref<const std::string&>().c_str());
	}

	ImGui::EndChild();
	ImGui::EndTabItem();
}
