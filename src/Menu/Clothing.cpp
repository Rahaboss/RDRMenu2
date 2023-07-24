#include "pch.h"
#include "Clothing.h"
#include "JobQueue.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Lists.h"

namespace Menu
{
	void RenderClothingMenu()
	{
		if (!ImGui::Begin("Clothing", g_Settings["render_clothing_menu"].get<bool*>()))
			ImGui::End();
		
		ImGui::Text("Scale");
		static float PlayerScale = 1.0f;
		ImGui::PushItemWidth(300.0f);
		if (ImGui::SliderFloat("###player_scale", &PlayerScale, 0.01f, 10.0f, "%.2f"))
		{
			QUEUE_JOB(=)
			{
				PED::_SET_PED_SCALE(g_LocalPlayer.m_Entity, PlayerScale);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Reset"))
		{
			QUEUE_JOB(=)
			{
				PlayerScale = 1.0f;
				PED::_SET_PED_SCALE(g_LocalPlayer.m_Entity, PlayerScale);
			}
			END_JOB()
		}
		ImGui::Separator();
		
		ImGui::BeginGroup();
		if (ImGui::Button("Remove Gunbelt", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("GUNBELTS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Knife Holster", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("HOLSTERS_KNIFE");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Right Holster", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("HOLSTERS_RIGHT");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Satchel Strap", ImVec2(180, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("SATCHEL_STRAPS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
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
				constexpr Hash Tag = rage::joaat("HATS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Left Holster", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("HOLSTERS_LEFT");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Satchel", ImVec2(170, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("SATCHELS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
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
		if (ImGui::Button("Remove Accessories", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("ACCESSORIES");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Belt", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("BELTS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Eyewear", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("EYEWEAR");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Gloves", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("GLOVES");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		
		ImGui::BeginGroup();
		if (ImGui::Button("Remove Hair", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("HAIR");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Crossdraw Holster", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("HOLSTERS_CROSSDRAW");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Loadouts", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("LOADOUTS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Large Mask", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("MASKS_LARGE");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		ImGui::EndGroup();
		ImGui::SameLine();

		ImGui::BeginGroup();
		if (ImGui::Button("Remove Neckwear", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("NECKWEAR");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Mask", ImVec2(0, 0)))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash Tag = rage::joaat("WEARABLE_MASKS");
				PED::REMOVE_TAG_FROM_META_PED(g_LocalPlayer.m_Entity, Tag, 1);
				PED::_UPDATE_PED_VARIATION(g_LocalPlayer.m_Entity, false, true, true, true, true);
			}
			END_JOB()
		}
		if (ImGui::Button("Remove Everything", ImVec2(0, 0)))
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
		
		if (ImGui::Button("Set Naked Outfit"))
		{
			QUEUE_JOB(=)
			{
				if (Features::IsArthurModel())
					PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, 14, false);
				else if (Features::IsJohnModel())
					PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, 28, false);
			}
			END_JOB()
		}
		if (Features::IsArthurModel())
		{
			ImGui::SameLine();
			if (ImGui::Button("Set Legend Of The East Outfit"))
			{
				QUEUE_JOB(=)
				{
					if (Features::IsArthurModel())
						PED::_EQUIP_META_PED_OUTFIT_PRESET(g_LocalPlayer.m_Entity, 13, false);
				}
				END_JOB()
			}
		}
		ImGui::Separator();
		
		if (ImGui::Button("Spawn Hat"))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash model = 813051418;
				Features::RequestModel(model);
				Object stick = OBJECT::CREATE_OBJECT_NO_OFFSET(model, 196.386f, 988.0386f, 189.1274f, true, true, false, true);
			}
			END_JOB()
		}
		ImGui::Separator();

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
		ImGui::SameLine();
		ImGui::Checkbox("p2 (Keep Accessories)", &KeepAcc);
		ImGui::Separator();
		
		RenderPlayerModelChanger();

		ImGui::End();
	}

	void RenderPlayerModelChanger()
	{
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
		ImGui::BeginChild("player_model_child", ImVec2(width, 200));
		for (const auto& p : g_PedList)
		{
			if (p.first.find(ModBuffer) == std::string::npos)
				continue;

			if (ImGui::Selectable(p.first.c_str()))
				Features::SetPlayerModel(p.second);
		}
		ImGui::EndChild();
		ImGui::SameLine();

		ImGui::BeginChild("player_model_preset_child", ImVec2(0, 200));
		if (ImGui::Selectable("Arthur"))
			Features::SetPlayerModel(rage::joaat("PLAYER_ZERO"));
		if (ImGui::Selectable("John"))
			Features::SetPlayerModel(rage::joaat("PLAYER_THREE"));
		if (ImGui::Selectable("Dutch"))
			Features::SetPlayerModel(rage::joaat("CS_DUTCH"));
		if (ImGui::Selectable("Robot"))
			Features::SetPlayerModel(rage::joaat("CS_CRACKPOTROBOT"));
		if (ImGui::Selectable("Naked Woman"))
			Features::SetPlayerModel(rage::joaat("U_F_M_RHDNUDEWOMAN_01"));
		if (ImGui::Selectable("Naked Man"))
			Features::SetPlayerModel(rage::joaat("RE_NAKEDSWIMMER_MALES_01"));
		if (ImGui::Selectable("Clan Member (1)"))
			Features::SetPlayerModel(rage::joaat("RE_RALLY_MALES_01"));
		if (ImGui::Selectable("Clan Member (2)"))
			Features::SetPlayerModel(rage::joaat("RE_RALLYDISPUTE_MALES_01"));
		if (ImGui::Selectable("Clan Member (3)"))
			Features::SetPlayerModel(rage::joaat("RE_RALLYSETUP_MALES_01"));
		if (ImGui::Selectable("2 Headed Skeleton (1)"))
			Features::SetPlayerModel(rage::joaat("U_M_M_CIRCUSWAGON_01"));
		if (ImGui::Selectable("2 Headed Skeleton (2)"))
			Features::SetPlayerModel(rage::joaat("U_F_M_CIRCUSWAGON_01"));
		if (ImGui::Selectable("Magnifico"))
			Features::SetPlayerModel(rage::joaat("CS_MAGNIFICO"));
		if (ImGui::Selectable("Bertram"))
			Features::SetPlayerModel(rage::joaat("CS_ODDFELLOWSPINHEAD"));
		if (ImGui::Selectable("Strange Man"))
			Features::SetPlayerModel(rage::joaat("CS_MYSTERIOUSSTRANGER"));
		if (ImGui::Selectable("Vampire"))
			Features::SetPlayerModel(rage::joaat("CS_VAMPIRE"));
		if (ImGui::Selectable("Swamp Freak"))
			Features::SetPlayerModel(rage::joaat("CS_SWAMPFREAK"));
		if (ImGui::Selectable("Jack"))
			Features::SetPlayerModel(rage::joaat("CS_JACKMARSTON"));
		if (ImGui::Selectable("Jack (Teen)"))
			Features::SetPlayerModel(rage::joaat("CS_JACKMARSTON_TEEN"));
		if (ImGui::Selectable("Gavin"))
			Features::SetPlayerModel(rage::joaat("CS_GAVIN"));
		if (ImGui::Selectable("Murder Mystery Body"))
			Features::SetPlayerModel(rage::joaat("RE_MURDERCAMP_MALES_01"));
		ImGui::EndChild();
	}
}
