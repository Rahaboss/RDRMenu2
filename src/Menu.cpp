#include "Menu.h"
#include "Thread/JobQueue.h"
#include "Rage/natives.h"
#include "Features.h"
#include "Thread/Thread.h"
#include "Script/World.h"
#include "Renderer/Renderer.h"
#include "Config/Settings.h"
#include "Config/Lists.h"
#include "Script/Player.h"

void Menu::RenderMenu()
{
	TRY
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
				RenderWeaponTab();
				RenderTeleportTab();
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
	EXCEPT{ LOG_EXCEPTION(); }
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

void Menu::RenderWeaponTab()
{
	if (!ImGui::BeginTabItem("Weapon"))
		return;

	ImGui::BeginChild("weapon_child");

	ImGui::Checkbox("Infinite Ammo", g_Settings["infinite_ammo"].get<bool*>());

	ImGui::EndChild();
	ImGui::EndTabItem();
}

void Menu::RenderTeleportTab()
{
	if (!ImGui::BeginTabItem("Teleport"))
		return;

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
