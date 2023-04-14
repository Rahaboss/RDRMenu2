#pragma once

#include "pch.h"
#include "Rendering.h"

namespace Menu
{
	void RenderRenderingTab()
	{
		if (!ImGui::BeginTabItem("Rendering"))
			return;

		ImGui::BeginChild("rendering_child");

		ImGui::Text("Player");
		ImGui::Checkbox("Player Bone ESP", g_Settings["esp"]["player_bone"].get<bool*>());
		ImGui::Checkbox("Player Bone ESP Debug", g_Settings["esp"]["player_bone_debug"].get<bool*>());
		ImGui::Checkbox("Player Box ESP", g_Settings["esp"]["player_box"].get<bool*>());
		ImGui::Checkbox("Player Model ESP", g_Settings["esp"]["player_model"].get<bool*>());
		ImGui::Checkbox("Player Health ESP", g_Settings["esp"]["player_health"].get<bool*>());
		ImGui::Separator();

		ImGui::Text("Added Peds");
		ImGui::Checkbox("Added Ped Bone ESP", g_Settings["esp"]["added_ped_bone"].get<bool*>());
		ImGui::Checkbox("Added Ped Box ESP", g_Settings["esp"]["added_ped_box"].get<bool*>());
		ImGui::Checkbox("Added Ped Model ESP", g_Settings["esp"]["added_ped_model"].get<bool*>());
		ImGui::Checkbox("Added Ped Health ESP", g_Settings["esp"]["added_ped_health"].get<bool*>());
		ImGui::Separator();

		ImGui::Text("Other");
		ImGui::Checkbox("Model Debug Gun", g_Settings["render_model_debug_gun"].get<bool*>());
		ImGui::Checkbox("RGB Electric Lantern", g_Settings["rgb_elec_lantern"].get<bool*>());

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
