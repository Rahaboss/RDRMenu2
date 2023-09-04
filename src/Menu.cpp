#include "Menu.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Features.h"
#include "Thread/Thread.h"
#include "Script/World.h"

void Menu::RenderMenu()
{
	ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;

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
	ImGui::SeparatorText("Test");

	static float f = 0.0f;
	static int counter = 0;

	ImGui::Text("This is some useful text.");
	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
	
	if (ImGui::Button("Button"))
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	if (ImGui::Button("Noon + Sunny"))
		JobQueue::Add(Script::NoonAndSunny);

	ImGui::EndChild();
	ImGui::EndTabItem();
}
