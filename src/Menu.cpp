#include "pch.h"
#include "Menu.h"

namespace Menu
{
	void RenderMenu()
	{
		TRY
		{
			if (IsOpen)
			{
				ImGui::SetNextWindowSize(ImVec2(1000, 750), ImGuiCond_FirstUseEver);
				ImGui::SetNextWindowPos(ImVec2(200, 100), ImGuiCond_FirstUseEver);
				if (ImGui::Begin("RDRMenu2", &IsOpen))
				{
					ImGui::BeginTabBar("tab_bar");
					RenderPlayerTab();
					RenderTeleportTab();
					RenderWeaponTab();
					RenderInventoryTab();
					RenderWorldTab();
					RenderSpawningTab();
					RenderRenderingTab();
					RenderCutsceneTab();
					RenderDebugTab();
					RenderLoggerTab();
					RenderMemoryTab();
					if (ImGui::BeginTabItem("Exit"))
					{
						Features::RunScriptCleanupAndExit();
						ImGui::EndTabItem();
					}
					ImGui::EndTabBar();
				}
				ImGui::End();

				if (g_Settings["enable_imgui_demo"].get<bool>())
					ImGui::ShowDemoWindow(g_Settings["enable_imgui_demo"].get<bool*>());

				if (g_Settings["render_clothing_menu"].get<bool>())
					RenderClothingMenu();
			}

			if (g_Settings["enable_overlay"].get<bool>())
				RenderOverlay();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
