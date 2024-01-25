#include "pch.h"
#include "Vehicle.h"
#include "Script/PlayerInfo.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/Entity.h"
#include "Rage/natives.h"

static void RenderVehicleButtons()
{
	if (ImGui::Button("Fix"))
	{
		QUEUE_JOB(=)
		{
			VEHICLE::SET_VEHICLE_FIXED(g_LocalPlayer.m_Vehicle);
		}
		END_JOB()
	}
}

static void RenderVehicleToggles()
{
	if (ImGui::Checkbox("Godmode", g_Settings["vehicle"]["godmode"].get<bool*>()))
	{
		if (!g_Settings["vehicle"]["godmode"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Script::SetEntityInvincible(g_LocalPlayer.m_Vehicle, false);
			}
			END_JOB()
		}
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Invisible", g_Settings["vehicle"]["invisible"].get<bool*>()))
	{
		if (!g_Settings["vehicle"]["invisible"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Script::SetEntityInvisible(g_LocalPlayer.m_Vehicle, false);
			}
			END_JOB()
		}
	}
}

void Menu::RenderVehicleTab()
{
	if (!ImGui::BeginTabItem("Vehicle"))
		return;

	ImGui::BeginChild("vehicle_child");

	ImGui::SeparatorText("Buttons");
	RenderVehicleButtons();

	ImGui::SeparatorText("Toggles");
	RenderVehicleToggles();

	ImGui::EndChild(); // vehicle_child
	ImGui::EndTabItem();
}
