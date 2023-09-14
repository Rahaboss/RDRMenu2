#include "pch.h"
#include "Overlay.h"
#include "Config/Settings.h"
#include "Script/PlayerInfo.h"

static void RenderOverlayMain()
{
	ImGuiIO& io = ImGui::GetIO();
	ImGui::Text("Framerate: %.1f (%.3f ms)", io.Framerate, 1000.0f / io.Framerate);
	ImGui::Text("Coords: %.2f, %.2f, %.2f", g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z);
	ImGui::Text("Heading: %.2f", g_LocalPlayer.m_Heading);
}

void Menu::RenderOverlay()
{
	ImGui::SetNextWindowPos(ImVec2(10, 10));
	ImGui::SetNextWindowBgAlpha(0.35f);
	ImGui::SetNextWindowSize(ImVec2(300, 100));

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
		ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoMove;

	if (ImGui::Begin("Overlay", g_Settings["render_overlay"].get<bool*>(), window_flags))
		RenderOverlayMain();
	ImGui::End();
}
