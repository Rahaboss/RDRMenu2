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

		ImGui::BeginChild("###left_pane_render", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
		ImGui::Text("Player");
		ImGui::Checkbox("Player Bone ESP", g_Settings["esp"]["player_bone"].get<bool*>());
		ImGui::Checkbox("Player Bone ESP Debug", g_Settings["esp"]["player_bone_debug"].get<bool*>());
		ImGui::Checkbox("Player Box ESP", g_Settings["esp"]["player_box"].get<bool*>());
		ImGui::Checkbox("Player Model ESP", g_Settings["esp"]["player_model"].get<bool*>());
		ImGui::Checkbox("Player Health ESP", g_Settings["esp"]["player_health"].get<bool*>());
		ImGui::Separator();

		ImGui::Text("Humans");
		ImGui::Checkbox("Human Bone ESP", g_Settings["esp"]["human_bone"].get<bool*>());
		ImGui::Checkbox("Human Box ESP", g_Settings["esp"]["human_box"].get<bool*>());
		ImGui::Checkbox("Human Model ESP", g_Settings["esp"]["human_model"].get<bool*>());
		ImGui::Checkbox("Human Health ESP", g_Settings["esp"]["human_health"].get<bool*>());
		ImGui::Checkbox("Human Ignore Dead", g_Settings["esp"]["human_ignore_dead"].get<bool*>());
		ImGui::Separator();

		ImGui::Text("Peds");
		ImGui::Checkbox("Ped Model ESP", g_Settings["esp"]["ped_model"].get<bool*>());
		ImGui::Checkbox("Ped Health ESP", g_Settings["esp"]["ped_health"].get<bool*>());
		ImGui::Checkbox("Ped Ignore Dead", g_Settings["esp"]["ped_ignore_dead"].get<bool*>());
		ImGui::Separator();

		ImGui::Text("Objects (Crashing likely)");
		ImGui::Checkbox("Object Model ESP", g_Settings["esp"]["object_model"].get<bool*>());
		ImGui::Checkbox("Pickup Model ESP", g_Settings["esp"]["pickup_model"].get<bool*>());
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("###right_pane_render");
		ImGui::Text("Vehicles");
		ImGui::Checkbox("Vehicle Model ESP", g_Settings["esp"]["vehicle_model"].get<bool*>());
		ImGui::Separator();

		ImGui::Text("Other");
		ImGui::Checkbox("Model Debug Gun", g_Settings["render_model_debug_gun"].get<bool*>());
		ImGui::Checkbox("RGB Electric Lantern", g_Settings["rgb_elec_lantern"].get<bool*>());
		ImGui::Checkbox("Enable ImGui Demo Window", g_Settings["enable_imgui_demo"].get<bool*>());
		ImGui::Checkbox("Enable Overlay", g_Settings["enable_overlay"].get<bool*>());
		ImGui::EndChild();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}
}
