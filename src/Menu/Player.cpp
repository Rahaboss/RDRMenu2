#include "pch.h"
#include "Player.h"
#include "Config/Settings.h"
#include "Thread/JobQueue.h"
#include "Script/PlayerInfo.h"
#include "Script/Ped.h"
#include "Rage/natives.h"
#include "Script/Entity.h"
#include "Script/Player.h"
#include "Config/Lists.h"
#include "Util/String.h"
#include "Script/Spawning.h"

static void RenderPlayerButtons()
{
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

	ImGui::SameLine();

	const bool DisableResetModel = Script::GetDefaultPlayerModel() == g_LocalPlayer.m_Model;
	if (DisableResetModel)
		ImGui::BeginDisabled();

	if (ImGui::Button("Reset Model"))
		JobQueue::Add(Script::ResetPlayerModel);

	if (DisableResetModel)
		ImGui::EndDisabled();

	if (ImGui::Button("Suicide"))
		JobQueue::Add(Script::Suicide);
	ImGui::SameLine();
	if (ImGui::Button("Good Honor"))
		JobQueue::Add(Script::SpawnGoodHonorEnemy);
	ImGui::SameLine();
	if (ImGui::Button("Bad Honor"))
		JobQueue::Add(Script::SpawnBadHonorEnemy);
}

static void RenderPlayerToggles()
{
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
	ImGui::Checkbox("Clean", g_Settings["player"]["clean"].get<bool*>());
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
	ImGui::Checkbox("No Sliding", g_Settings["player"]["no_sliding"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Super Run", g_Settings["player"]["super_run"].get<bool*>());

	ImGui::Checkbox("Never Wanted", g_Settings["never_wanted"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Super Jump", g_Settings["player"]["super_jump"].get<bool*>());
	ImGui::SameLine();
	if (ImGui::Checkbox("Invisible", g_Settings["player"]["invisible"].get<bool*>()))
	{
		if (!g_Settings["player"]["invisible"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				Script::SetEntityInvisible(g_LocalPlayer.m_Entity, false);
			}
			END_JOB()
		}
	}
}

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
			Script::SetMoney(static_cast<int>(s_MoneyAmount * 100.0));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Add###add_money"))
	{
		QUEUE_JOB(=)
		{
			Script::AddMoney(static_cast<int>(s_MoneyAmount * 100.0));
		}
		END_JOB()
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove###remove_money"))
	{
		QUEUE_JOB(=)
		{
			Script::RemoveMoney(static_cast<int>(s_MoneyAmount * 100.0));
		}
		END_JOB()
	}
}

void Menu::RenderPlayerTab()
{
	if (!ImGui::BeginTabItem("Player"))
		return;

	ImGui::BeginChild("player_child");

	TRY
	{
		ImGui::SeparatorText("Buttons");
		RenderPlayerButtons();

		ImGui::SeparatorText("Toggles");
		RenderPlayerToggles();

		ImGui::SeparatorText("Money");
		RenderMoneyChanger();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild();
	ImGui::EndTabItem();
}
