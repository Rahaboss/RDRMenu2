#include "pch.h"
#include "Menu.h"
#include "Rendering.h"
#include "Player.h"
#include "Mount.h"
#include "Weapon.h"
#include "World.h"
#include "Teleport.h"
#include "Spawning.h"
#include "Debug.h"
#include "Cutscene.h"
#include "Renderer/RGB.h"
#include "Script/Features.h"
#include "ESP/ESP.h"
#include "Util/Timer.h"
#include "Config/Settings.h"

void Menu::Render()
{
	TRY
	{
		Timer t;

		UpdateMenu();

		if (IsOpen)
			RenderMenu();

		Timer::s_MenuTime = t.GetMillis();

		ESP::RenderESP();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Menu::UpdateMenu()
{
	TRY
	{
		ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;
		Renderer::RGBTick();
		ImVec4 title_bg = Renderer::GetImGuiRGBA(); title_bg.x *= 0.6f; title_bg.y *= 0.6f; title_bg.z *= 0.6f;
		ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = title_bg;
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

static void RenderExitTab()
{
	if (ImGui::BeginTabItem("Exit"))
	{
		Features::StartExit();
		ImGui::EndTabItem();
	}
}

void Menu::RenderMenu()
{
	TRY
	{
		static bool s_WindowSetup = false;
		if (!s_WindowSetup)
		{
			constexpr float f = 850.0f;
			ImGui::SetNextWindowSize(ImVec2(f, 600), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x - 100 - f, 100), ImGuiCond_FirstUseEver);
		}

		if (ImGui::Begin("RDRMenu2", &IsOpen, ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginTabBar("tab_bar"))
			{
				RenderPlayerTab();
				RenderMountTab();
				RenderWeaponTab();
				RenderWorldTab();
				RenderTeleportTab();
				RenderSpawningTab();
				RenderCutsceneTab();
				RenderRenderingTab();
				RenderDebugTab();
				RenderExitTab();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();

		if (!s_WindowSetup)
		{
			ImGui::SetNextWindowPos(ImVec2(100, 100), ImGuiCond_FirstUseEver);
			s_WindowSetup = true;
		}

		if (g_Settings["render_imgui_demo"].get<bool>())
			ImGui::ShowDemoWindow(g_Settings["render_imgui_demo"].get<bool*>());
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
