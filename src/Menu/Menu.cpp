#include "pch.h"
#include "Menu.h"
#include "Rendering.h"
#include "Player.h"
#include "Mount.h"
#include "Vehicle.h"
#include "Weapon.h"
#include "World.h"
#include "Teleport.h"
#include "Spawning.h"
#include "DebugMenu/Debug.h"
#include "Network.h"
#include "Cutscene.h"
#include "Inventory.h"
#include "Overlay.h"
#include "Renderer/RGB.h"
#include "Script/Features.h"
#include "ESP/ESP.h"
#include "Util/Timer.h"
#include "Config/Settings.h"
#include "Model.h"

static void UpdateMenu()
{
	TRY
	{
		ImGui::GetIO().MouseDrawCursor = Menu::IsOpen;
		Renderer::RGBTick();
		ImVec4 TitleBG = Renderer::GetImGuiRGBA(); TitleBG.x *= 0.6f; TitleBG.y *= 0.6f; TitleBG.z *= 0.6f;
		ImGui::GetStyle().Colors[ImGuiCol_TitleBgActive] = TitleBG;
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

static void SetWindowParamsFirstTime()
{
	constexpr float f = 1020;
	constexpr float g = 40;
	ImGui::SetNextWindowSize(ImVec2(f, 650), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(ImGui::GetMainViewport()->WorkSize.x - g - f, g), ImGuiCond_FirstUseEver);
}

static void RenderMenu()
{
	TRY
	{
		SetWindowParamsFirstTime();

		if (ImGui::Begin("RDRMenu2", &Menu::IsOpen, ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginTabBar("tab_bar"))
			{
				Menu::RenderPlayerTab();
				Menu::RenderMountTab();
				Menu::RenderVehicleTab();
				Menu::RenderWeaponTab();
				Menu::RenderWorldTab();
				Menu::RenderTeleportTab();
				Menu::RenderSpawningTab();
				Menu::RenderCutsceneTab();
				Menu::RenderInventoryTab();
				Menu::RenderRenderingTab();
				Menu::RenderModelTab();
#if ENABLE_DEBUG_TAB
				Menu::RenderDebugTab();
#endif // ENABLE_DEBUG_TAB
#if ENABLE_NETWORK_TAB
				Menu::RenderNetworkTab();
#endif // ENABLE_NETWORK_TAB
				RenderExitTab();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	EXCEPT{ LOG_EXCEPTION(); Menu::IsOpen = false; }
}

void Menu::Render()
{
	TRY
	{
		Timer MenuTimer;

		UpdateMenu();

		if (IsOpen)
			RenderMenu();

		if (g_Settings["render_overlay"].get<bool>())
			RenderOverlay();

#if !_DIST
		if (IsOpen && g_Settings["render_imgui_demo"].get<bool>())
			ImGui::ShowDemoWindow(g_Settings["render_imgui_demo"].get<bool*>());
#endif // _DIST

		Timer::s_MenuTime = MenuTimer.GetMillis();

		ESP::RenderESP();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
