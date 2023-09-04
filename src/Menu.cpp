#include "Menu.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Features.h"
#include "Thread/Thread.h"
#include "Script/World.h"
#include "Renderer/Renderer.h"
#include "Config/Settings.h"

void Menu::RenderMenu()
{
	ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;

	if (IsOpen)
	{
		if (ImGui::Begin("RDRMenu2", &IsOpen))
		{
			ImGui::BeginTabBar("tab_bar");
			RenderTestTab();
			RenderDebugTab();
			RenderWorldTab();
			if (ImGui::BeginTabItem("Exit"))
			{
				Features::StartExit();
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

void Menu::RenderDebugTab()
{
	if (!ImGui::BeginTabItem("Debug"))
		return;

	ImGui::BeginChild("debug_child");

	if (ImGui::CollapsingHeader("Settings JSON"))
	{
		ImGui::PushFont(Renderer::DefaultFont);

		ImGui::Text(g_Settings.dump(4).c_str());

		ImGui::PopFont();
	}

	ImGui::EndChild();
	ImGui::EndTabItem();
}

void Menu::RenderWorldTab()
{
	if (!ImGui::BeginTabItem("World"))
		return;

	ImGui::BeginChild("world_child");

	ImGui::Checkbox("Disable West Elizabeth Pinkerton Patrols", g_Settings["disable_pinkerton_patrols"].get<bool*>());
	ImGui::Checkbox("Enable All DLCs", g_Settings["enable_dlcs"].get<bool*>());

	ImGui::EndChild();
	ImGui::EndTabItem();
}
