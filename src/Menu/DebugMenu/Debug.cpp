#include "pch.h"
#include "./Debug.h"
#include "./General.h"
#include "./Interior.h"
#include "./Memory.h"
#include "./Speech.h"
#include "./Notify.h"
#include "./Scenario.h"

void Menu::RenderDebugTab()
{
	if (!ImGui::BeginTabItem("Debug"))
		return;

	ImGui::BeginChild("debug_child");

	if (ImGui::BeginTabBar("debug_tab_bar"))
	{
		TRY
		{
			DebugMenu::RenderGeneralTab();
			DebugMenu::RenderInteriorMenu();
			DebugMenu::RenderMemoryTab();
			DebugMenu::RenderSpeechTab();
			DebugMenu::RenderNotifyTab();
			DebugMenu::RenderScenarioTab();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	ImGui::EndTabBar(); // debug_tab_bar

	ImGui::EndChild(); // debug_child
	ImGui::EndTabItem();
}
