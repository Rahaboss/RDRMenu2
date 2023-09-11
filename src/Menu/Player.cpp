#include "pch.h"
#include "Player.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/PlayerInfo.h"
#include "Script/Ped.h"
#include "Rage/natives.h"
#include "Script/Entity.h"
#include "Script/Player.h"

static void RenderMoneyChanger()
{
	static double s_MoneyAmount = 10000.00;
	ImGui::SetNextItemWidth(200.0f);
	ImGui::InputDouble("##money_input", &s_MoneyAmount, 0, 0, "$%.2f");
	ImGui::SameLine();
	if (ImGui::Button("Get###get_money"))
	{
		QUEUE_JOB(=)
		{
			s_MoneyAmount = (double)Script::GetMoney() / 100.0;
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Set###set_money"))
	{
		QUEUE_JOB(=)
		{
			Script::SetMoney((int)(s_MoneyAmount * 100.0));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Add###add_money"))
	{
		QUEUE_JOB(=)
		{
			Script::AddMoney((int)(s_MoneyAmount * 100.0));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove###remove_money"))
	{
		QUEUE_JOB(=)
		{
			Script::RemoveMoney((int)(s_MoneyAmount * 100.0));
		}
		END_JOB()
	}
}

void Menu::RenderPlayerTab()
{
	if (!ImGui::BeginTabItem("Player"))
		return;

	ImGui::BeginChild("player_child");

	ImGui::SeparatorText("Buttons");
	if (ImGui::Button("Fill Cores"))
	{
		QUEUE_JOB(=)
		{
			Script::FillCores(g_LocalPlayer.m_Entity);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Give Gold Cores"))
	{
		QUEUE_JOB(=)
		{
			Script::GiveGoldCores(g_LocalPlayer.m_Entity);
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Explode Head"))
	{
		QUEUE_JOB(=)
		{
			PED::EXPLODE_PED_HEAD(g_LocalPlayer.m_Entity, RAGE_JOAAT("WEAPON_SNIPERRIFLE_CARCANO"));
		}
		END_JOB()
	}
	ImGui::SameLine();

	const bool DisableSetOntoLastMount = !g_LocalPlayer.m_LastMount || g_LocalPlayer.m_Mount == g_LocalPlayer.m_LastMount;
	if (DisableSetOntoLastMount)
		ImGui::BeginDisabled();

	if (ImGui::Button("Set Onto Mount"))
		JobQueue::Add(Script::SetOnLastMount);

	if (DisableSetOntoLastMount)
		ImGui::EndDisabled();

	ImGui::SeparatorText("Toggles");
	if (ImGui::Checkbox("Godmode", g_Settings["player"]["godmode"].get<bool*>()))
	{
		if (!g_Settings["player"]["godmode"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Script::SetEntityInvincible(g_LocalPlayer.m_Entity, false);
			}
			END_JOB()
		}
	}
	ImGui::SameLine();
	ImGui::Checkbox("Gold Cores", g_Settings["player"]["gold_cores"].get<bool*>());
	ImGui::SameLine();
	if (ImGui::Checkbox("No Ragdoll", g_Settings["player"]["no_ragdoll"].get<bool*>()))
	{
		if (!g_Settings["player"]["no_ragdoll"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Script::SetPedNoRagdoll(g_LocalPlayer.m_Entity, false);
			}
			END_JOB()
		}
	}
	ImGui::SameLine();
	ImGui::Checkbox("Never Wanted", g_Settings["never_wanted"].get<bool*>());

	ImGui::SeparatorText("Money");
	RenderMoneyChanger();

	ImGui::EndChild();
	ImGui::EndTabItem();
}
