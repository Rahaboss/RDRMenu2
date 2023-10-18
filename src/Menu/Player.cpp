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

	ImGui::Checkbox("Clean", g_Settings["player"]["clean"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Super Jump", g_Settings["player"]["super_jump"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("Super Run", g_Settings["player"]["super_run"].get<bool*>());
	ImGui::SameLine();
	ImGui::Checkbox("No Sliding", g_Settings["player"]["no_sliding"].get<bool*>());
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

static void RenderModelChanger()
{
	static char s_ModelFilter[200]{};
	ImGui::SetNextItemWidth(230.0f);
	ImGui::InputText("Filter Models", s_ModelFilter, IM_ARRAYSIZE(s_ModelFilter));
	ImGui::SameLine();
	if (ImGui::Button("Clear##clear_model"))
		s_ModelFilter[0] = '\0';
	ImGui::Separator();

	ImGui::BeginChild("##app_left_inner");
	std::string FilterUpper = Util::StringToUpperCopy(s_ModelFilter);
	for (const auto& p : Lists::PedList)
	{
		if (p.first.find(FilterUpper) == std::string::npos)
			continue;

		if (ImGui::Selectable(p.first.c_str(), g_LocalPlayer.m_Model == p.second))
		{
			QUEUE_JOB(=)
			{
				Script::SetPlayerModel(p.second);
			}
			END_JOB()
		}
	}
	ImGui::EndChild(); // ##app_left_inner
}

static void RenderOutfitPresetList()
{
	ImGui::AlignTextToFramePadding();
	ImGui::TextUnformatted("Set Outfit Preset:");
	ImGui::SameLine();

	ImGui::PushButtonRepeat(true);

	static int s_OutfitPreset = 0;
	if (ImGui::ArrowButton("##outfit_left", ImGuiDir_Left))
	{
		QUEUE_JOB(=)
		{
			s_OutfitPreset--;
			s_OutfitPreset = std::clamp(s_OutfitPreset, 0, PED::GET_NUM_META_PED_OUTFITS(g_LocalPlayer.m_Entity) - 1);
			Script::SetPedOutfitPreset(g_LocalPlayer.m_Entity, s_OutfitPreset);
		}
		END_JOB()
	}
	ImGui::SameLine();

	ImGui::AlignTextToFramePadding();
	ImGui::Text("%d", s_OutfitPreset);
	ImGui::SameLine();

	if (ImGui::ArrowButton("##outfit_right", ImGuiDir_Right))
	{
		QUEUE_JOB(=)
		{
			s_OutfitPreset++;
			s_OutfitPreset = std::clamp(s_OutfitPreset, 0, PED::GET_NUM_META_PED_OUTFITS(g_LocalPlayer.m_Entity) - 1);
			Script::SetPedOutfitPreset(g_LocalPlayer.m_Entity, s_OutfitPreset);
		}
		END_JOB()
	}

	ImGui::PopButtonRepeat();
}

static void RenderMetaPedOutfitList()
{
	static Hash s_SelectedModel = 0;
	static std::string s_SelectedModelName{};

	if (s_SelectedModel != g_LocalPlayer.m_Model)
	{
		s_SelectedModel = g_LocalPlayer.m_Model;
		s_SelectedModelName = Util::StringToUpperCopy(Lists::GetHashName(s_SelectedModel));
	}
	
	const json& Outfits{ Lists::MetaPedOutfits[s_SelectedModelName] };
	if (Outfits.is_array())
	{
		for (size_t i = 0; i < Outfits.size(); i++)
		{
			std::string Name;
			Hash Model;

			if (Outfits[i].is_string())
			{
				Name = Outfits[i].get_ref<const std::string&>();
				Model = rage::joaat(Name);
			}
			else if (Outfits[i].is_number())
			{
				Model = Outfits[i].get<uint32_t>();
				Name = std::to_string(Model);
			}

			if (!Name.empty())
			{
				if (ImGui::Selectable(Name.c_str(), PED::_IS_META_PED_OUTFIT_EQUIPPED(g_LocalPlayer.m_Entity, Model)))
				{
					QUEUE_JOB(=)
					{
						Script::SetMetaPedOutfit(g_LocalPlayer.m_Entity, Model);
					}
					END_JOB()
				}
			}
		}
	}
}

static void RenderPlayerAppearance()
{
	ImGui::BeginChild("##player_appearance");

	ImGui::BeginChild("##app_left", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));

	ImGui::SeparatorText("Set Model");
	RenderModelChanger();

	ImGui::EndChild(); // ##app_left
	ImGui::SameLine();
	ImGui::BeginChild("##app_right");

	ImGui::SeparatorText("Set Outfit");
	RenderOutfitPresetList();
	ImGui::Separator();

	ImGui::BeginChild("##app_right_inner");
	RenderMetaPedOutfitList();
	ImGui::EndChild(); // ##app_right_inner

	ImGui::EndChild(); // ##app_right
	ImGui::EndChild(); // ##player_appearance
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

		RenderPlayerAppearance();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	ImGui::EndChild();
	ImGui::EndTabItem();
}
