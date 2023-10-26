#include "pch.h"
#include "Mount.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/Entity.h"
#include "Script/PlayerInfo.h"
#include "Script/Ped.h"

static void RenderMountButtons()
{

}

static void RenderMountToggles()
{
	if (ImGui::Checkbox("Godmode", g_Settings["mount"]["godmode"].get<bool*>()))
	{
		if (!g_Settings["mount"]["godmode"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Script::SetEntityInvincible(g_LocalPlayer.m_Mount, false);
			}
			END_JOB()
		}
	}
	ImGui::SameLine();
	ImGui::Checkbox("Gold Cores", g_Settings["mount"]["gold_cores"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Clean", g_Settings["mount"]["clean"].get<bool*>());
	ImGui::SameLine();
	if (ImGui::Checkbox("No Ragdoll", g_Settings["mount"]["no_ragdoll"].get<bool*>()))
	{
		if (g_LocalPlayer.m_Mount && !g_Settings["mount"]["no_ragdoll"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Script::SetPedNoRagdoll(g_LocalPlayer.m_Mount, false);
			}
			END_JOB()
		}
	}
	ImGui::SameLine();
	ImGui::Checkbox("No Sliding", g_Settings["mount"]["no_sliding"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Super Run", g_Settings["mount"]["super_run"].get<bool*>());
}

static void RenderMountMain()
{
	TRY
	{
		// ImGui::SeparatorText("Buttons");
		// RenderMountButtons();

		ImGui::SeparatorText("Toggles");
		RenderMountToggles();
	}
	EXCEPT{ LOG_EXCEPTION(); }
}

void Menu::RenderMountTab()
{
	if (!ImGui::BeginTabItem("Mount"))
		return;

	ImGui::BeginChild("mount_child");

	TRY
	{
		RenderMountMain();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild();
	ImGui::EndTabItem();
}
