#include "Menu.h"

void Menu::RenderMenu()
{
	if (IsOpen)
	{
		if (ImGui::Begin("RDRMenu2", &IsOpen))
		{
			ImGui::BeginTabBar("tab_bar");
			RenderTestTab();
			if (ImGui::BeginTabItem("Exit"))
			{
				g_Running = false;
				ImGui::EndTabItem();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
}

void Menu::RenderTestTab()
{
	if (!ImGui::BeginTabItem("Test"))
		return;

	ImGui::BeginChild("test_child");
	ImGui::Text("Test");
	
	ImGui::EndChild();
	ImGui::EndTabItem();
}
