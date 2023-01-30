#include "pch.h"
#include "Overlay.h"
#include "PlayerInfo.h"

namespace Menu
{
	void RenderOverlay()
	{
		TRY
		{
			bool* p_open = g_Settings["enable_overlay"].get<bool*>();
			static int location = 0;
			ImGuiIO& io = ImGui::GetIO();
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
			if (location >= 0)
			{
				const float PAD = 10.0f;
				const ImGuiViewport* viewport = ImGui::GetMainViewport();
				ImVec2 work_pos = viewport->WorkPos; // Use work area to avoid menu-bar/task-bar, if any!
				ImVec2 work_size = viewport->WorkSize;
				ImVec2 window_pos, window_pos_pivot;
				window_pos.x = (location & 1) ? (work_pos.x + work_size.x - PAD) : (work_pos.x + PAD);
				window_pos.y = (location & 2) ? (work_pos.y + work_size.y - PAD) : (work_pos.y + PAD);
				window_pos_pivot.x = (location & 1) ? 1.0f : 0.0f;
				window_pos_pivot.y = (location & 2) ? 1.0f : 0.0f;
				ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
				window_flags |= ImGuiWindowFlags_NoMove;
			}
			else if (location == -2)
			{
				// Center window
				ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Always, ImVec2(0.5f, 0.5f));
				window_flags |= ImGuiWindowFlags_NoMove;
			}
			ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
			if (ImGui::Begin("###overlay", p_open, window_flags))
			{
				RenderOverlayMain();
				if (ImGui::BeginPopupContextWindow())
				{
					if (ImGui::MenuItem("Custom", NULL, location == -1)) location = -1;
					if (ImGui::MenuItem("Center", NULL, location == -2)) location = -2;
					if (ImGui::MenuItem("Top-left", NULL, location == 0)) location = 0;
					if (ImGui::MenuItem("Top-right", NULL, location == 1)) location = 1;
					if (ImGui::MenuItem("Bottom-left", NULL, location == 2)) location = 2;
					if (ImGui::MenuItem("Bottom-right", NULL, location == 3)) location = 3;
					if (p_open && ImGui::MenuItem("Close")) *p_open = false;
					ImGui::EndPopup();
				}
			}
			ImGui::End();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RenderOverlayMain()
	{
		TRY
		{
			ImGuiIO& io = ImGui::GetIO();
			ImGui::Text("Framerate: %.1f (%.3f ms)", io.Framerate, 1000.0f / io.Framerate);
			ImGui::Text("Coords: %.2f, %.2f, %.2f", g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z);
			ImGui::Text("Heading: %.2f", g_LocalPlayer.m_Heading);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
