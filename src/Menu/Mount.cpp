#include "pch.h"
#include "Mount.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/Entity.h"
#include "Script/PlayerInfo.h"

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
}

static void RenderMountMain()
{
	TRY
	{
		//ImGui::SeparatorText("Buttons");
		//RenderMountButtons();

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
