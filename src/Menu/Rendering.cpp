#include "pch.h"
#include "Rendering.h"
#include "Config/Settings.h"

void Menu::RenderRenderingTab()
{
	if (!ImGui::BeginTabItem("Rendering"))
		return;

	ImGui::BeginChild("rendering_child");

	if (ImGui::BeginChild("left_half", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
	{
		{
			ImGui::SeparatorText("Ped ESP");
			ImGui::Checkbox("Enable##enable_ped", g_Settings["esp"]["ped"]["enable"].get<bool*>());
			const bool PedDisabled = !g_Settings["esp"]["ped"]["enable"].get<bool>();

			if (PedDisabled)
				ImGui::BeginDisabled();

			ImGui::Checkbox("Model##enable_ped_model", g_Settings["esp"]["ped"]["model"].get<bool*>());

			if (PedDisabled)
				ImGui::EndDisabled();
		}

		{
			ImGui::SeparatorText("Local Player ESP");
			ImGui::Checkbox("Enable##enable_local_player", g_Settings["esp"]["player"]["enable"].get<bool*>());
			const bool LocalPlayerDisabled = !g_Settings["esp"]["player"]["enable"].get<bool>();

			if (LocalPlayerDisabled)
				ImGui::BeginDisabled();

			ImGui::Checkbox("Model##enable_local_player_model", g_Settings["esp"]["player"]["model"].get<bool*>());
			ImGui::Checkbox("Bone##enable_local_player_bone", g_Settings["esp"]["player"]["bone"].get<bool*>());

			if (LocalPlayerDisabled)
				ImGui::EndDisabled();
		}

		{
			ImGui::SeparatorText("Pickup ESP");
			ImGui::Checkbox("Enable##enable_pickup", g_Settings["esp"]["pickup"]["enable"].get<bool*>());
			const bool PickupDisabled = !g_Settings["esp"]["pickup"]["enable"].get<bool>();

			if (PickupDisabled)
				ImGui::BeginDisabled();

			ImGui::Checkbox("Model##enable_pickup_model", g_Settings["esp"]["pickup"]["model"].get<bool*>());

			if (PickupDisabled)
				ImGui::EndDisabled();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();

	if (ImGui::BeginChild("right_half"))
	{
		{
			ImGui::SeparatorText("Vehicle ESP");
			ImGui::Checkbox("Enable##enable_vehicle", g_Settings["esp"]["vehicle"]["enable"].get<bool*>());
			const bool VehicleDisabled = !g_Settings["esp"]["vehicle"]["enable"].get<bool>();

			if (VehicleDisabled)
				ImGui::BeginDisabled();

			ImGui::Checkbox("Model##enable_vehicle_model", g_Settings["esp"]["vehicle"]["model"].get<bool*>());

			if (VehicleDisabled)
				ImGui::EndDisabled();
		}

		{
			ImGui::SeparatorText("Object ESP (Crashing Likely)");
			ImGui::Checkbox("Enable##enable_object", g_Settings["esp"]["object"]["enable"].get<bool*>());
			const bool ObjectDisabled = !g_Settings["esp"]["object"]["enable"].get<bool>();

			if (ObjectDisabled)
				ImGui::BeginDisabled();

			ImGui::Checkbox("Model##enable_object_model", g_Settings["esp"]["object"]["model"].get<bool*>());

			if (ObjectDisabled)
				ImGui::EndDisabled();
		}
	}
	ImGui::EndChild();

	ImGui::EndChild();
	ImGui::EndTabItem();
}
