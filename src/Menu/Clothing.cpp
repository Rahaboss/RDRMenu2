#include "pch.h"
#include "Clothing.h"
#include "JobQueue.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Lists.h"

// These menus have a weird bug. When the Clothing menu is open and you change
// the player's model an exception is thrown in the native 0xED40380076A31506 (PLAYER::SET_PLAYER_MODEL).
// For example in Menu::RenderScaleTab if you remove the call to ImGui::SliderFloat
// then there is no exception. The exception is thrown in the native, not in the menu code...

namespace Menu
{
	void RenderClothingMenu()
	{
		ImGui::Begin("Clothing", g_Settings["render_clothing_menu"].get<bool*>());

		ImGui::BeginTabBar("###clothing_menu_tab_bar");
		RenderScaleTab();
		RenderRemoveItemTab();
		RenderPlayerModelChanger();
		RenderPlayerOutfitChanger();
		RenderScenarioTab();
		ImGui::EndTabBar();
		
		ImGui::End();
	}

	void RenderScaleTab()
	{
		if (!ImGui::BeginTabItem("Scale"))
			return;
		
		ImGui::PushItemWidth(300.0f);
		static float s_PlayerScale = 1.0f;
		// This line causes an exception in PLAYER::SET_PLAYER_MODEL for some reason
		if (ImGui::SliderFloat("###player_scale", &s_PlayerScale, 0.01f, 10.0f, "%.2f"))
			Features::SetPedScale(g_LocalPlayer.m_Entity, s_PlayerScale);
		ImGui::PopItemWidth();
		if (ImGui::Button("Reset"))
		{
			s_PlayerScale = 1.0f;
			Features::SetPedScale(g_LocalPlayer.m_Entity, s_PlayerScale);
		}

		ImGui::EndTabItem();
	}

	void RenderRemoveItemTab()
	{
		if (!ImGui::BeginTabItem("Remove Item"))
			return;

		ImGui::BeginGroup();
		if (ImGui::Button("Remove Gunbelt", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("GUNBELTS"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Knife Holster", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("HOLSTERS_KNIFE"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Right Holster", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("HOLSTERS_RIGHT"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Satchel Strap", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("SATCHEL_STRAPS"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		if (ImGui::Button("Remove Hat", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("HATS"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Left Holster", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("HOLSTERS_LEFT"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Satchel", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("SATCHELS"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Knock Off Hat", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				PED::KNOCK_OFF_PED_PROP(g_LocalPlayer.m_Entity, false, true, false, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		if (ImGui::Button("Remove Accessories"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("ACCESSORIES"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Belt"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("BELTS"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Eyewear"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("EYEWEAR"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Gloves"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("GLOVES"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		
		ImGui::BeginGroup();
		if (ImGui::Button("Remove Hair"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("HAIR"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Crossdraw Holster"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("HOLSTERS_CROSSDRAW"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Loadouts"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("LOADOUTS"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Large Mask"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("MASKS_LARGE"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		if (ImGui::Button("Remove Neckwear"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("NECKWEAR"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Mask"))
		{
			QUEUE_JOB(=)
			{
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, RAGE_JOAAT("WEARABLE_MASKS"), 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Everything"))
		{
			QUEUE_JOB(=)
			{
				for (int i = 0; i < PED::_GET_NUM_COMPONENT_CATEGORIES_IN_PED(g_LocalPlayer.m_Entity); i++)
				{
					Hash category = PED::_GET_PED_COMPONENT_CATEGORY_BY_INDEX(g_LocalPlayer.m_Entity, i);
					if (PED::_IS_META_PED_USING_COMPONENT(g_LocalPlayer.m_Entity, category))
						PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, category, 1);
				}
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();

		ImGui::EndTabItem();
	}

	void RenderPlayerModelChanger()
	{
		if (!ImGui::BeginTabItem("Change Model"))
			return;

		ImGui::AlignTextToFramePadding();
		ImGui::Text("Set Player Model");
		ImGui::SameLine();
		static char ModBuffer[200];
		ImGui::SetNextItemWidth(250.0f);
		ImGui::InputText("###filter_ped", ModBuffer, 200, ImGuiInputTextFlags_CharsUppercase);
		ImGui::SameLine();
		if (ImGui::Button("Set###set_model_manually"))
			Features::SetPlayerModel(rage::joaat(ModBuffer));
		ImGui::SameLine();
		if (ImGui::Button("Reset###reset_model"))
			Features::ResetPlayerModel();

		const float width = ImGui::GetContentRegionAvail().x / 2;
		ImGui::BeginChild("player_model_child", ImVec2(width, 0));
		for (const auto& p : g_PedList)
		{
			if (p.first.find(ModBuffer) == std::string::npos)
				continue;

			if (ImGui::Selectable(p.first.c_str(), p.second == g_LocalPlayer.m_Model))
				Features::SetPlayerModel(p.second);
		}
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("player_model_preset_child");
		if (ImGui::Selectable("Arthur"))
			Features::SetPlayerModel(RAGE_JOAAT("PLAYER_ZERO"));
		if (ImGui::Selectable("John"))
			Features::SetPlayerModel(RAGE_JOAAT("PLAYER_THREE"));
		if (ImGui::Selectable("Dutch"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_DUTCH"));
		if (ImGui::Selectable("Robot"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_CRACKPOTROBOT"));
		if (ImGui::Selectable("Naked Woman"))
			Features::SetPlayerModel(RAGE_JOAAT("U_F_M_RHDNUDEWOMAN_01"));
		if (ImGui::Selectable("Naked Man"))
			Features::SetPlayerModel(RAGE_JOAAT("RE_NAKEDSWIMMER_MALES_01"));
		if (ImGui::Selectable("Clan Member (1)"))
			Features::SetPlayerModel(RAGE_JOAAT("RE_RALLY_MALES_01"));
		if (ImGui::Selectable("Clan Member (2)"))
			Features::SetPlayerModel(RAGE_JOAAT("RE_RALLYDISPUTE_MALES_01"));
		if (ImGui::Selectable("Clan Member (3)"))
			Features::SetPlayerModel(RAGE_JOAAT("RE_RALLYSETUP_MALES_01"));
		if (ImGui::Selectable("2 Headed Skeleton (1)"))
			Features::SetPlayerModel(RAGE_JOAAT("U_M_M_CIRCUSWAGON_01"));
		if (ImGui::Selectable("2 Headed Skeleton (2)"))
			Features::SetPlayerModel(RAGE_JOAAT("U_F_M_CIRCUSWAGON_01"));
		if (ImGui::Selectable("Magnifico"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_MAGNIFICO"));
		if (ImGui::Selectable("Bertram"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_ODDFELLOWSPINHEAD"));
		if (ImGui::Selectable("Strange Man"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_MYSTERIOUSSTRANGER"));
		if (ImGui::Selectable("Vampire"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_VAMPIRE"));
		if (ImGui::Selectable("Swamp Freak"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_SWAMPFREAK"));
		if (ImGui::Selectable("Jack"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_JACKMARSTON"));
		if (ImGui::Selectable("Jack (Teen)"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_JACKMARSTON_TEEN"));
		if (ImGui::Selectable("Gavin"))
			Features::SetPlayerModel(RAGE_JOAAT("CS_GAVIN"));
		if (ImGui::Selectable("Murder Mystery Body"))
			Features::SetPlayerModel(RAGE_JOAAT("RE_MURDERCAMP_MALES_01"));
		ImGui::EndChild();
		
		ImGui::EndTabItem();
	}
	
	void RenderPlayerOutfitChanger()
	{
		if (!ImGui::BeginTabItem("Change Outfit"))
			return;
		
		ImGui::BeginChild("meta_ped_outfit_child", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
		
		static Hash s_SelectedModel = 0;
		static json s_SelectedOutfits;
		if (s_SelectedModel != g_LocalPlayer.m_Model)
		{
			s_SelectedModel = g_LocalPlayer.m_Model;

			std::string ModelName(Features::GetPedModelName(s_SelectedModel));
			Features::StringToLower(ModelName);

			s_SelectedOutfits = g_MetaPedOutfits[ModelName];
		}

		for (const auto& o : s_SelectedOutfits)
		{
			if (o.is_string())
			{
				const auto& name = o.get_ref<const std::string&>();
				Hash model = rage::joaat(name);

				if (ImGui::Selectable(name.c_str()))
					Features::SetMetaPedOutfit(g_LocalPlayer.m_Entity, model);
			}
			else if (o.is_number())
			{
				Hash model = o.get<uint32_t>();

				if (ImGui::Selectable(std::to_string(model).c_str()))
					Features::SetMetaPedOutfit(g_LocalPlayer.m_Entity, model);
			}
		}
		
		ImGui::EndChild();
		ImGui::SameLine();
		
		ImGui::BeginGroup();
		ImGui::Text("Set Outfit");
		static int Outfit = 0;
		static bool KeepAcc = false;
		
		ImGui::PushButtonRepeat(true);
		if (ImGui::ArrowButton("###lof", ImGuiDir_Left))
		{
			QUEUE_JOB(=)
			{
				if (Outfit == 0)
					++Outfit;
				PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, --Outfit, KeepAcc);
			}
			END_JOB()
		}
		ImGui::SameLine();
		
		ImGui::AlignTextToFramePadding();
		ImGui::Text("%d", Outfit);
		ImGui::SameLine();
		
		if (ImGui::ArrowButton("###rof", ImGuiDir_Right))
		{
			QUEUE_JOB(=)
			{
				if (Outfit == PED::GET_NUM_META_PED_OUTFITS(g_LocalPlayer.m_Entity) - 1)
					--Outfit;
				PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, ++Outfit, KeepAcc);
			}
			END_JOB()
		}
		ImGui::PopButtonRepeat();
		
		ImGui::Checkbox("p2 (Keep Accessories)", &KeepAcc);
		ImGui::EndGroup();

		ImGui::EndTabItem();
	}

	void RenderScenarioTab()
	{
		if (!ImGui::BeginTabItem("Scenarios"))
			return;

		static Hash s_SelectedScenario = g_ScenarioList.begin()->second;
		ImGui::BeginChild("###scenario_left", ImVec2(ImGui::GetContentRegionAvail().x / 2, 0));
		for (const auto& s : g_ScenarioList)
		{
			if (ImGui::Selectable(s.first.c_str(), s.second == s_SelectedScenario))
				s_SelectedScenario = s.second;
		}
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("###scenario_right");
		if (ImGui::Button("Play Scenario"))
			Features::StartPedScenario(g_LocalPlayer.m_Entity, s_SelectedScenario);
		if (ImGui::Button("Stop Scenario"))
			Features::StopPedScenario(g_LocalPlayer.m_Entity);
		if (ImGui::Button("Stop Scenario Immediately/Clear Props"))
		{
			QUEUE_JOB(=)
			{
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(g_LocalPlayer.m_Entity, true, true);
			}
			END_JOB()
		}
		ImGui::EndChild();

		ImGui::EndTabItem();
	}
}
