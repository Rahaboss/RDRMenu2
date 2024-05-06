#include "pch.h"
#include "Rendering.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/Rendering.h"

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
			ImGui::Checkbox("Bone##enable_ped_bone", g_Settings["esp"]["ped"]["bone"].get<bool*>());
			ImGui::Checkbox("Box##enable_ped_box", g_Settings["esp"]["ped"]["box"].get<bool*>());
			ImGui::Checkbox("Ignore Dead##enable_ped_ignore_dead", g_Settings["esp"]["ped"]["ignore_dead"].get<bool*>());

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
#if !DIST
			ImGui::Checkbox("Bone Debug##enable_local_player_bone_debug", g_Settings["esp"]["player"]["bone_debug"].get<bool*>());
#endif // !DIST
			ImGui::Checkbox("Box##enable_local_player_box", g_Settings["esp"]["player"]["box"].get<bool*>());

			if (LocalPlayerDisabled)
				ImGui::EndDisabled();
		}

		{
			ImGui::SeparatorText("Animal ESP");
			ImGui::Checkbox("Enable##enable_animal", g_Settings["esp"]["animal"]["enable"].get<bool*>());
			const bool AnimalDisabled = !g_Settings["esp"]["animal"]["enable"].get<bool>();

			if (AnimalDisabled)
				ImGui::BeginDisabled();

			ImGui::Checkbox("Model##enable_animal_model", g_Settings["esp"]["animal"]["model"].get<bool*>());
			ImGui::Checkbox("Ignore Dead##enable_animal_ignore_dead", g_Settings["esp"]["animal"]["ignore_dead"].get<bool*>());

			if (AnimalDisabled)
				ImGui::EndDisabled();
		}
	}
	ImGui::EndChild();
	ImGui::SameLine();

	if (ImGui::BeginChild("right_half"))
	{
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
			ImGui::SeparatorText("Object ESP");
			ImGui::Checkbox("Enable##enable_object", g_Settings["esp"]["object"]["enable"].get<bool*>());
			const bool ObjectDisabled = !g_Settings["esp"]["object"]["enable"].get<bool>();

			if (ObjectDisabled)
				ImGui::BeginDisabled();

			ImGui::Checkbox("Model##enable_object_model", g_Settings["esp"]["object"]["model"].get<bool*>());

			if (ObjectDisabled)
				ImGui::EndDisabled();
		}

		{
			ImGui::SeparatorText("Other");
			ImGui::Checkbox("RGB Electric Lantern", g_Settings["weapon"]["rgb_electric_lantern"].get<bool*>());
			if (ImGui::Checkbox("Disable HUD", g_Settings["disable_hud"].get<bool*>()))
			{
				if (!g_Settings["disable_hud"].get<bool>())
				{
					QUEUE_JOB(=)
					{
						Script::DisableHUD(false);
					}
					END_JOB()
				}
			}
			ImGui::Checkbox("Disable Black Borders", g_Settings["disable_black_borders"].get<bool*>());
			ImGui::Checkbox("Render Overlay", g_Settings["render_overlay"].get<bool*>());
#ifndef _DIST
			ImGui::Checkbox("Render Guarma Border", g_Settings["render_guarma_border"].get<bool*>());
#endif // _DIST
		}
	}
	ImGui::EndChild();

	ImGui::EndChild();
	ImGui::EndTabItem();
}
