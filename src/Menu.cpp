#include "Menu.h"
#include "JobQueue.h"
#include "rage/natives.h"

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

	ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

	ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

	if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		counter++;
	ImGui::SameLine();
	ImGui::Text("counter = %d", counter);

	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

	if (ImGui::Button("Noon + Sunny"))
	{
		QUEUE_JOB(=)
		{
			CLOCK::SET_CLOCK_TIME(12, 0, 0);
			MISC::SET_WEATHER_TYPE(RAGE_JOAAT("SUNNY"), true, true, false, 0.0f, false);
		}
		END_JOB()
	}
	
	ImGui::EndChild();
	ImGui::EndTabItem();
}
