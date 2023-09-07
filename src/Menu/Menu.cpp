#include "pch.h"
#include "Menu.h"
#include "Rendering.h"
#include "Player.h"
#include "Weapon.h"
#include "World.h"
#include "Teleport.h"
#include "Spawning.h"
#include "Debug.h"
#include "Renderer/RGB.h"
#include "Script/Features.h"
#include "ESP/ESP.h"

void Menu::Render()
{
	TRY
	{
		UpdateMenu();

		if (IsOpen)
			RenderMenu();

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
		if (ImGui::Begin("RDRMenu2", &IsOpen, ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::BeginTabBar("tab_bar"))
			{
				RenderPlayerTab();
				RenderWeaponTab();
				RenderWorldTab();
				RenderTeleportTab();
				RenderSpawningTab();
				RenderRenderingMenu();
				RenderDebugTab();
				RenderExitTab();
			}
			ImGui::EndTabBar();
		}
		ImGui::End();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}
