#include "pch.h"
#include "Debug.h"
#include "PlayerInfo.h"
#include "JobQueue.h"
#include "ScriptGlobal.h"
#include "Signature.h"
#include "Lists.h"
#include "Features.h"
#include "CutsceneHelper.h"
#include "ImGuiAddons.h"

namespace Menu
{
	void RenderDebugTab()
	{
		if (!ImGui::BeginTabItem("Debug"))
			return;
		
		ImGui::BeginChild("debug_child");

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Debug Info"))
			RenderDebugInfo();
		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Debug Toggles"))
			RenderDebugToggles();
		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Debug Buttons"))
			RenderDebugButtons();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Coord Debug"))
			RenderCoordDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Cutscene Debug"))
			RenderCutsceneDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Native Debug"))
			RenderNativeDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Ped Debug"))
			RenderPedDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Text Debug"))
			RenderTextDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("HUD Context Debug"))
			RenderHUDContextDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("CPed Debug"))
			RenderCPedDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("PersChar Debug"))
			RenderPersCharDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Explosion Debug"))
			RenderExplosionDebug();

		ImGui::EndChild();
		ImGui::EndTabItem();
	}

	void RenderCoordDebug()
	{
		Vector3 DebugPos = g_LocalPlayer.m_Pos;
		bool DebugTeleport = false;
		constexpr float ArrowButtonSpeed = 50.0f;
		ImGui::PushItemWidth(200.0f);
		ImGui::PushButtonRepeat(true);
		
		if (ImGui::ArrowButton("###debug_x-", ImGuiDir_Left))
		{
			DebugTeleport = true;
			DebugPos.x -= ArrowButtonSpeed;
		}
		ImGui::SameLine();
		DebugTeleport |= ImGui::DragFloat("X###debug_x", &DebugPos.x);
		ImGui::SameLine();
		if (ImGui::ArrowButton("###debug_x+", ImGuiDir_Right))
		{
			DebugTeleport = true;
			DebugPos.x += ArrowButtonSpeed;
		}

		if (ImGui::ArrowButton("###debug_y-", ImGuiDir_Left))
		{
			DebugTeleport = true;
			DebugPos.y -= ArrowButtonSpeed;
		}
		ImGui::SameLine();
		DebugTeleport |= ImGui::DragFloat("Y###debug_y", &DebugPos.y);
		ImGui::SameLine();
		if (ImGui::ArrowButton("###debug_y+", ImGuiDir_Right))
		{
			DebugTeleport = true;
			DebugPos.y += ArrowButtonSpeed;
		}

		if (ImGui::ArrowButton("###debug_z-", ImGuiDir_Left))
		{
			DebugTeleport = true;
			DebugPos.z -= ArrowButtonSpeed;
		}
		ImGui::SameLine();
		DebugTeleport |= ImGui::DragFloat("Z###debug_z", &DebugPos.z);
		ImGui::SameLine();
		if (ImGui::ArrowButton("###debug_z+", ImGuiDir_Right))
		{
			DebugTeleport = true;
			DebugPos.z += ArrowButtonSpeed;
		}

		ImGui::PopButtonRepeat();
		ImGui::PopItemWidth();
		if (DebugTeleport)
		{
			QUEUE_JOB(=)
			{
				ENTITY::SET_ENTITY_COORDS(g_LocalPlayer.m_Entity, DebugPos.x, DebugPos.y, DebugPos.z - 1, false, false, false, false);
			}
			END_JOB()
		}
		if (ImGui::Checkbox("Freeze Position", g_Settings["freeze_player"].get<bool*>()) && !g_Settings["freeze_player"].get<bool>())
		{
			QUEUE_JOB(=)
			{
				ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Entity, false);
			}
			END_JOB()
		}

		if (ImGui::Button("Copy Coords"))
			LOG_TO_CLIPBOARD("%.2ff, %.2ff, %.2ff", g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z);
		ImGui::SameLine();
		ImGui::Text("%.2f, %.2f, %.2f", g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z);

		ImGui::SameLine();
		ImGui::ColorButton("RGB Color", Features::GetImGuiRGB(), ImGuiColorEditFlags_Uint8);
	}

	void RenderCutsceneDebug()
	{
		static int s_CurrentCutscene = 0;
		static char CutFilter[200]{ "cutscene@" };
		ImGui::BeginGroup();

		ImGui::Text("Select Cutscene:");
		if (ImGui::BeginChild("###cutscene_list", ImVec2(350, 250)))
		{
			std::string s1(CutFilter);
			std::transform(s1.cbegin(), s1.cend(), s1.begin(), ::tolower);

			for (int i = 0; i < g_Cutscenes.size(); i++)
			{
				if (g_Settings["disable_default_cutscenes"].get<bool>())
				{
					int cs_size = static_cast<int>(g_Cutscenes[i].size());
					if (g_Cutscenes[i].contains("player_model"))
						cs_size--;
					if (g_Cutscenes[i].contains("player_outfit_preset"))
						cs_size--;

					if (cs_size <= 1)
						continue;
				}

				const std::string& Name = g_Cutscenes[i]["id"].get_ref<const std::string&>();

				std::string s2(Name);
				std::transform(s2.cbegin(), s2.cend(), s2.begin(), ::tolower);
				if (s2.find(s1) == std::string::npos)
					continue;

				if (ImGui::Selectable(Name.c_str(), i == s_CurrentCutscene))
					s_CurrentCutscene = i;
			}
		}
		ImGui::EndChild();

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		
		ImGui::Text("Filter Cutscenes:");
		if (ImGui::BeginChild("###cutscene_info", ImVec2(0, 250)) && !g_Cutscenes.empty())
		{
			ImGui::PushItemWidth(300);
			ImGui::InputText("###filter_cut", CutFilter, 200);
			ImGui::PopItemWidth();
		
			ImGui::Checkbox("Disable Default Cutscenes", g_Settings["disable_default_cutscenes"].get<bool*>());
			ImGui::Checkbox("Disable Cinematic Black Borders", g_Settings["no_black_borders"].get<bool*>());
			ImGui::Separator();

			const nlohmann::json& CurrentJson = g_Cutscenes[s_CurrentCutscene];
			if (ImGui::Button("Play"))
			{
				QUEUE_JOB(&)
				{
					Features::PlayCutsceneFromJson(CurrentJson);
				}
				END_JOB()
			}
			ImGui::SameLine();
			if (ImGui::Button("Reload List"))
			{
				QUEUE_JOB(=)
				{
					Lists::ReloadCutscenesList();
				}
				END_JOB()
			}

			ImGui::Text("Cutscene Info:");
			ImGui::Text("animDict: %s", CurrentJson["id"].get_ref<const std::string&>().c_str());
			if (CurrentJson.size() != 1)
			{
				if (CurrentJson.contains("playback_id"))
				{
					ImGui::Text("playbackListName: %s", CurrentJson["playback_id"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("player_model"))
				{
					ImGui::Text("Player Model: %s", CurrentJson["player_model"].get_ref<const std::string&>().c_str());
				}
			
				if (CurrentJson.contains("peds"))
				{
					ImGui::Text("Cutscene Peds:");
					for (const auto& cs_ped : CurrentJson["peds"])
						ImGui::BulletText(cs_ped["name"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("objects"))
				{
					ImGui::Text("Cutscene Objects:");
					for (const auto& cs_ped : CurrentJson["objects"])
						ImGui::BulletText(cs_ped["name"].get_ref<const std::string&>().c_str());
				}
				if (CurrentJson.contains("vehicles"))
				{
					ImGui::Text("Cutscene Vehicles:");
					for (const auto& cs_ped : CurrentJson["vehicles"])
						ImGui::BulletText(cs_ped["name"].get_ref<const std::string&>().c_str());
				}
			}
		}
		ImGui::EndChild();

		ImGui::EndGroup();
		ImGui::Separator();

		ImGui::Text("Beta / Special Cutscenes");
		if (ImGui::Button("A Test of Faith II"))
			Features::PlayDinoLadyCutscene();
		ImGui::SameLine();
		if (ImGui::Button("A Fisher of Fish II"))
			Features::PlayFishCollectorCutscene();
		ImGui::SameLine();
		if (ImGui::Button("The Gilded Cage"))
			Features::PlayIndustryCutscene();
		ImGui::SameLine();
		if (ImGui::Button("Jack Cutscene"))
			Features::PlayJackCutscene();
		
		if (ImGui::Button("Money Lending and Other Sins"))
			Features::PlayDebtCollectorCutscene();
		ImGui::SameLine();
		if (ImGui::Button("Annesburg Jail Breakout With Charles"))
			Features::PlayAnnesburgBreakoutCutscene();

		if (ImGui::Button("The Fine Art of Conversation"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayCutsceneFromID("cutscene@ntv2_ext");
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Charles Leaving To Help Natives"))
			Features::PlayCharlesLeavingCutscene();

		if (ImGui::Button("Beecher's Hope"))
			Features::PlayBeechersHopeCutscene();
		ImGui::SameLine();
		if (ImGui::Button("Bare Knuckle Friendships"))
			Features::PlayBareKnuckleFriendshipsCutscene();
		ImGui::SameLine();
		if (ImGui::Button("A New Future Imagined"))
			Features::PlayANewFutureImaginedCutscene();

		if (ImGui::Button("Ending Strawberry"))
			Features::PlayEndingStrawberryCutscene();
	}

	void RenderDebugButtons()
	{
		if (ImGui::Button("Change Player Model"))
			Features::SetPlayerModel();
		ImGui::SameLine();
		if (ImGui::Button("Reset Player Model"))
			Features::ResetPlayerModel();

		if (ImGui::Button("Knock Off Ped Prop"))
		{
			QUEUE_JOB(=)
			{
				PED::KNOCK_OFF_PED_PROP(g_LocalPlayer.m_Entity, false, true, false, true);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Reload Lists"))
		{
			QUEUE_JOB(=)
			{
				Lists::Reload();
			}
			END_JOB()
		}
		
		if (ImGui::Button("Get Component Categories"))
		{
			QUEUE_JOB(=)
			{
				LOG_TO_CONSOLE("=== BEGIN METAPED COMPONENT CATEGORIES ===\n");

				const int num = PED::_GET_NUM_COMPONENT_CATEGORIES_IN_PED(g_LocalPlayer.m_Entity);
				for (int i = 0; i < num; i++)
					LOG_TO_CONSOLE("%u\n", PED::_GET_PED_COMPONENT_CATEGORY_BY_INDEX(g_LocalPlayer.m_Entity, i));

				LOG_TO_CONSOLE("=== END METAPED COMPONENT CATEGORIES ===\n");
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Add Blip"))
		{
			QUEUE_JOB(=)
			{
				constexpr Hash hash = joaat("blip_cash_bag");
				MAP::BLIP_ADD_FOR_COORDS(hash, g_LocalPlayer.m_Pos.x, g_LocalPlayer.m_Pos.y, g_LocalPlayer.m_Pos.z);
			}
			END_JOB()
		}

		if (ImGui::Button("Change Is Session Started"))
		{
			if (Pointers::IsSessionStarted)
				*Pointers::IsSessionStarted = !(*Pointers::IsSessionStarted);
		}
		ImGui::SameLine();
		if (ImGui::Button("Toggle Saint Denis Interiors"))
		{
			int iVar0;

			iVar0 = INTERIOR::GET_INTERIOR_AT_COORDS(2638.399f, -1290.197f, 51.2461f);
			if (INTERIOR::IS_VALID_INTERIOR(iVar0))
			{
				if (INTERIOR::IS_INTERIOR_READY(iVar0))
				{
					auto func_2060 = [](int iParam0)
					{
						int iVar0;
						int iVar1;
						int iVar2;

						iVar0 = iParam0;
						iVar1 = (iVar0 / 31);
						iVar2 = (iVar0 % 31);

						auto Global_1934765_f_21 = ScriptGlobal(1934765).At(21).Get<int*>();
						MISC::SET_BIT(&(Global_1934765_f_21[iVar1]), iVar2);
						MISC::SET_BIT(&(Global_1934765_f_21[iVar1 * 2]), iVar2);

						auto Global_1934765_f_30 = ScriptGlobal(1934765).At(30).Get<int*>();
						MISC::SET_BIT(&(Global_1934765_f_30[iVar1]), iVar2);
						MISC::SET_BIT(&(Global_1934765_f_30[iVar1 * 2]), iVar2);
					};
					
					func_2060(134);
					func_2060(137);

					INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(iVar0, "new_com_bank_before", 0);
					INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(iVar0, "new_com_bank_after", 0);
					INTERIOR::_0x2533F2AB0EB9C6F9(iVar0, 1);
					INTERIOR::_0xFE2B3D5500B1B2E4(iVar0, 1);
				}
			}
		}

		if (ImGui::Button("Sort Cutscenes JSON"))
		{
			std::sort(g_Cutscenes.begin(), g_Cutscenes.end(), [](const nlohmann::json& a, const nlohmann::json& b) {
				return a["id"].get_ref<const std::string&>() < b["id"].get_ref<const std::string&>();
			});

			std::fstream File(Features::GetConfigPath().append("cut.json"), std::fstream::out | std::fstream::trunc);
			assert(File.good());
			File << g_Cutscenes.dump(4);
			File.close();
		}
		ImGui::SameLine();
		if (ImGui::Button("Mrs. Adler"))
		{
			QUEUE_JOB(=)
			{
				Ped ped = Features::SpawnPed(CS_MRSADLER);
				Features::SetMetapedOutfit(ped, 0x5F43C3FC);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Noon And Sunny"))
		{
			Features::SetClockTime(12);
			Features::SetWeatherType(SUNNY);
		}

		if (ImGui::Button("Activate Grand Korrigan Interiors"))
		{
			QUEUE_JOB(=)
			{
				//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(2552.205f, -1170.773f, 52.6838f, joaat("NEW_TAILOR01_INT_MAIN")), 0);
				//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(2552.205f, -1170.773f, 52.6838f, joaat("NEW_TAILOR01_INT_MAIN")), 0);
				//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(2654.253f, -1179.118f, 52.2785f, joaat("NEW_BARBER_INT")), 0);
				//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(2654.533f, -1178.978f, 52.2785f, joaat("NEW_BARBER_INT")), 0);
				//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(3286.869f, -1304.827f, 50.7904f, joaat("SER_GRANDKOR_L2_INT")), 0);
				//INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(3288.151f, -1300.511f, 50.9101f, joaat("SER_GRANDKOR_L2_INT")), 0);
				INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(3288.815f, -1309.385f, 41.7978f, joaat("SER_GRANDKOR_L1_INT")), "korrigan_props_poker", 0);
				INTERIOR::ACTIVATE_INTERIOR_ENTITY_SET(INTERIOR::GET_INTERIOR_AT_COORDS_WITH_TYPEHASH(3289.768f, -1309.406f, 41.792f, joaat("SER_GRANDKOR_L1_INT")), "korrigan_props_default", 0);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Spawn Hat Test"))
		{
			QUEUE_JOB(=)
			{
				Features::SpawnObject(813051418);
			}
			END_JOB()
		}

		if (ImGui::Button("ImGui Cherry Style"))
		{
			ImGui::StyleColorsCherry();
		}
		ImGui::SameLine();
		if (ImGui::Button("Make Lists"))
		{
			std::filesystem::path Path(Features::GetConfigPath().append("lists.json"));
			//std::fstream File(Path, std::fstream::out | std::fstream::trunc);
			std::fstream File(Path, std::fstream::in);
			
			if (File.good())
			{
				nlohmann::json j;
				File >> j;
				File.close();

				nlohmann::json jj;
				jj["obj"] = std::vector<std::string>{};
				jj["peds"] = std::vector<std::string>{};
				jj["unknown"] = std::vector<std::string>{};
				jj["vehicles"] = std::vector<std::string>{};

				for (const auto& o : j["obj"])
				{
					Hash h = o.get<Hash>();

					bool found = false;
					for (const auto& o2 : g_ObjectList)
					{
						if (o2.second == h)
						{
							jj["obj"].push_back(o2.first);
							found = true;
						}
					}

					if (!found)
						jj["obj"].push_back(std::to_string(h));
				}

				for (const auto& o : j["peds"])
				{
					Hash h = o.get<Hash>();

					bool found = false;
					for (const auto& o2 : g_PedList)
					{
						if (o2.second == h)
						{
							jj["peds"].push_back(o2.first);
							found = true;
						}
					}
					if (!found)
						jj["peds"].push_back(std::to_string(h));

				}

				for (const auto& o : j["vehicles"])
				{
					Hash h = o.get<Hash>();

					bool found = false;
					for (const auto& o2 : g_VehicleList)
					{
						if (o2.second == h)
						{
							jj["vehicles"].push_back(o2.first);
							found = true;
						}
					}
					if (!found)
						jj["vehicles"].push_back(std::to_string(h));

				}

				jj["unknown"] = j["unknown"];

				File.open(Path, std::fstream::out | std::fstream::trunc);
				File << jj.dump(4);
				File.close();
			}
		}

		if (ImGui::Button("Make Lists2"))
		{
			std::vector<const char*> names{};
			std::vector<Hash> hashes;
			for (const auto& n : names)
				hashes.push_back(joaat(n));

			std::filesystem::path Path(Features::GetConfigPath().append("lists2.json"));
			//std::fstream File(Path, std::fstream::out | std::fstream::trunc);
			std::fstream File(Path, std::fstream::in);

			if (File.good())
			{
				nlohmann::json j, jj;
				File >> j;
				File.close();

				jj["obj"] = std::vector<std::string>{};
				jj["peds"] = std::vector<std::string>{};
				//jj["unknown"] = std::vector<std::string>{};

				for (const auto& p : j["peds"])
				{
					bool found = false;
					for (int i = 0; i < hashes.size(); i++)
					{
						if (hashes[i] == p.get<Hash>())
						{
							found = true;
							jj["peds"].push_back(names[i]);
						}
					}
					if (!found)
						jj["peds"].push_back(std::to_string(p.get<Hash>()));
				}

				for (const auto& p : j["obj"])
				{
					bool found = false;
					for (int i = 0; i < hashes.size(); i++)
					{
						if (hashes[i] == p.get<Hash>())
						{
							found = true;
							jj["obj"].push_back(names[i]);
						}
					}
					if (!found)
						jj["obj"].push_back(std::to_string(p.get<Hash>()));
				}

				File.open(Path, std::fstream::out | std::fstream::trunc);
				File << jj.dump(4);
				File.close();
			}
		}
		ImGui::SameLine();
		if (ImGui::Button("Make Lists3"))
		{
            QUEUE_JOB(=)
            {
                [=]() {
			        LOG_TO_CONSOLE("Make Lists3\n");
			        std::filesystem::path Path(Features::GetConfigPath().append("lists3.json"));
			        std::fstream File(Path, std::fstream::out | std::fstream::trunc);

			        if (File.good())
			        {
				        nlohmann::json j;
				        j["peds"] = std::vector<Hash>{};
				        j["vehicles"] = std::vector<Hash>{};
				        j["obj"] = std::vector<Hash>{};
				        j["unknown"] = std::vector<Hash>{};

				        std::vector<Hash> p, v, o, u;
				        for (uint64_t i = 0; i < 0xFFFFFFFF; i++)
				        {
					        Hash h = (Hash)i;

                            if (i % 0xFFFFFF == 0)
                                Features::YieldThread();

					        //if (!Features::IsModelValid(h))
					        //	continue;

					        //LOG_TO_CONSOLE("Logging hash: %u\n", h);
                            if (STREAMING::IS_MODEL_A_PED(h))
                            {
                                j["peds"].push_back(h);
                                LOG_TO_CONSOLE("Ped: %u\n", h);
                            }
                            else if (STREAMING::IS_MODEL_A_VEHICLE(h))
                            {
                                j["vehicles"].push_back(h);
                                LOG_TO_CONSOLE("Veh: %u\n", h);
                            }
                            else if (STREAMING::_IS_MODEL_AN_OBJECT(h))
                            {
                                j["obj"].push_back(h);
                                LOG_TO_CONSOLE("Obj: %u\n", h);
                            }
                            else if (h == 2053922531)
                            {
                                LOG_TO_CONSOLE("2053922531 Failed!\n");
                            }
					        //else
                            //    j["unknown"].push_back(h);
				        }

				        //j["peds"] = p;
				        //j["vehicles"] = v;
				        //j["obj"] = o;
				        //j["unknown"] = u;

				        File << j.dump(4);
				        File.close();
			        }
                }();
            }
            END_JOB()
		}
	}

	void RenderDebugInfo()
	{
		ImGui::BeginGroup();

		if (Pointers::IsSessionStarted)
			ImGui::Text("Is Session Started: %s", *Pointers::IsSessionStarted ? "true" : "false");

		ImGui::Text("\xE2\x84\xAE \xE2\x84\x85 \xE2\x88\x91 \xE2\x86\x95 \xC6\xB1");
		ImGui::Text("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9\n");

		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::AlignTextToFramePadding();
		ImGui::Text("CPed: 0x%llX", g_LocalPlayer.m_Ped);
		ImGui::SameLine();
		if (ImGui::Button("Copy Address"))
			LOG_TO_CLIPBOARD("%llX", g_LocalPlayer.m_Ped);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("GetEntityAddress: 0x%llX", Pointers::GetEntityAddress(g_LocalPlayer.m_Entity));
		ImGui::SameLine();
		if (ImGui::Button("Copy Address###copy_gea"))
			LOG_TO_CLIPBOARD("%llX", Pointers::GetEntityAddress(g_LocalPlayer.m_Entity));
		
		if (g_LocalPlayer.m_Ped)
		{
			ImGui::AlignTextToFramePadding();
			ImGui::Text("sub_7FF73D8DB3C4: 0x%llX", Pointers::sub_7FF73D8DB3C4(g_LocalPlayer.m_Ped));
			ImGui::SameLine();
			if (ImGui::Button("Copy Address###copy_sub3c4"))
				LOG_TO_CLIPBOARD("%llX", Pointers::sub_7FF73D8DB3C4(g_LocalPlayer.m_Ped));
		}

		ImGui::AlignTextToFramePadding();
		ImGui::Text("RDR2.exe: 0x%llX", g_BaseAddress);
		ImGui::SameLine();
		if (ImGui::Button("Copy Address###base_address"))
			LOG_TO_CLIPBOARD("%llX", g_BaseAddress);

		ImGui::EndGroup();
	}

	void RenderDebugToggles()
	{
		ImGui::BeginGroup();
		
		ImGui::Checkbox("Log Ped Spawning", g_Settings["logging"]["spawned_ped"].get<bool*>());
		ImGui::Checkbox("Log Human Spawning", g_Settings["logging"]["spawned_human"].get<bool*>());
		ImGui::Checkbox("Log Vehicle Spawning", g_Settings["logging"]["spawned_vehicle"].get<bool*>());
		ImGui::Checkbox("Log Object Spawning", g_Settings["logging"]["spawned_object"].get<bool*>());
		
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Log Added Inventory Items", g_Settings["logging"]["added_inventory_item"].get<bool*>());
		ImGui::Checkbox("Log Created Cutscenes", g_Settings["logging"]["created_cutscene"].get<bool*>());
		ImGui::Checkbox("Log Set Decor", g_Settings["logging"]["set_decor"].get<bool*>());
		ImGui::Checkbox("Log Added Cutscene Entities", g_Settings["logging"]["added_cutscene_entity"].get<bool*>());
		
		ImGui::EndGroup();
		ImGui::SameLine();
		ImGui::BeginGroup();
		
		ImGui::Checkbox("Enable ImGui Demo Window", g_Settings["enable_imgui_demo"].get<bool*>());
		ImGui::Checkbox("Enable Overlay", g_Settings["enable_overlay"].get<bool*>());
		if (ImGui::Checkbox("Enable Freecam", &EnableFreeCam) && !EnableFreeCam)
		{
			QUEUE_JOB(=)
			{
				CAM::SET_CAM_ACTIVE(CamEntity, false);
				CAM::RENDER_SCRIPT_CAMS(false, true, 500, true, true, 0);
				CAM::DESTROY_CAM(CamEntity, false);
				STREAMING::SET_FOCUS_ENTITY(g_LocalPlayer.m_Entity);

				ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Vehicle, false);

				CamEntity = 0;
			}
			END_JOB()
		}
		
		ImGui::EndGroup();
	}

	void RenderNativeDebug()
	{
		ImGui::Text("Native Address Lookup");

		static uint64_t nhash = 0xFA925AC00EB830B9; // 0xBD5DD5EAE2B6CE14; // 0xB980061DA992779D; // 0xED40380076A31506; // 0xA86D5F069399F44D; // 0x25ACFC650B65C538;
		static uintptr_t addr = (uintptr_t)NativeContext::GetHandler(nhash); // Address in current process
		static uintptr_t off = addr - g_BaseAddress; // Offset from imagebase
		static uintptr_t ida_addr = off + 0x7FF73CAB0000; // Address in process dump in IDA

		ImGui::PushItemWidth(250);
		if (ImGui::InputU64("Native Hash", &nhash, 1, 100, ImGuiInputTextFlags_CharsHexadecimal | ImGuiInputTextFlags_CharsUppercase))
		{
			addr = (uintptr_t)NativeContext::GetHandler(nhash); // Address in current process
			off = addr - g_BaseAddress; // Offset from imagebase
			ida_addr = off + 0x7FF73CAB0000; // Address in process dump in IDA
		}
		ImGui::PopItemWidth();

		if (addr)
		{
			ImGui::Text("0x%llX Handler Address (RDR2.exe + 0x%llX):", nhash, off);
		
			ImGui::AlignTextToFramePadding();
			ImGui::Text("Current Address: %llX", addr);
			ImGui::SameLine();
			if (ImGui::Button("Copy Current Address"))
				LOG_TO_CLIPBOARD("%llX", addr);

			ImGui::AlignTextToFramePadding();
			ImGui::Text("IDA Address: %llX", ida_addr);
			ImGui::SameLine();
			if (ImGui::Button("Copy IDA Address"))
				LOG_TO_CLIPBOARD("%llX", ida_addr);
		}
		else
		{
			ImGui::TextDisabled("Unknown Native: 0x%llX", nhash);
		}
	}

	void RenderPedDebug()
	{
		const bool PedSpawned = PedDebug.ent;

		ImGui::PushItemWidth(350.0f);
		ImGui::BeginGroup();

		// Ped controls
		if (ImGui::Button("Spawn"))
		{
			QUEUE_JOB(=)
			{
				PedDebug.ent = Features::SpawnPed(PedDebug.model);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(PedDebug.model);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (!PedSpawned)
			ImGui::BeginDisabled();
		if (ImGui::Button("Despawn"))
		{
			QUEUE_JOB(=)
			{
				Features::DeletePed(PedDebug.ent);
				PedDebug.ent = 0;
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Resurrect"))
		{
			QUEUE_JOB(=)
			{
				PED::RESURRECT_PED(PedDebug.ent);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Revive"))
		{
			QUEUE_JOB(=)
			{
				PED::REVIVE_INJURED_PED(PedDebug.ent);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Random Component Variations"))
		{
			QUEUE_JOB(=)
			{
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(PedDebug.ent, 0);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Random Outfit Variations"))
		{
			QUEUE_JOB(=)
			{
				PED::_SET_RANDOM_OUTFIT_VARIATION(PedDebug.ent, true);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Good Honor"))
		{
			QUEUE_JOB(=)
			{
				DECORATOR::DECOR_SET_INT(PedDebug.ent, "honor_override", -9999);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Set Bad Honor"))
		{
			QUEUE_JOB(=)
			{
				DECORATOR::DECOR_SET_INT(PedDebug.ent, "honor_override", 9999);
			}
			END_JOB()
		}

		if (ImGui::Button("TP Player To Ped"))
			Features::TeleportOnGround(PedDebug.pos);
		ImGui::SameLine();
		if (ImGui::Button("TP Ped To Player"))
		{
			QUEUE_JOB(=)
			{
				const Vector3& pos = g_LocalPlayer.m_Pos;
				ENTITY::SET_ENTITY_COORDS(PedDebug.ent, pos.x, pos.y, pos.z, false, false, false, false);
			}
			END_JOB()
		}
		if (!PedSpawned)
			ImGui::EndDisabled();

		ImGui::EndGroup();
		ImGui::PopItemWidth();
		ImGui::SameLine();
		ImGui::BeginGroup();

		ImGui::Checkbox("Freeze", &PedDebug.freeze);
		ImGui::Checkbox("Invincible", &PedDebug.invincible);
		ImGui::Checkbox("Visible", &PedDebug.visible);

		ImGui::EndGroup();
		ImGui::Separator();
		ImGui::BeginGroup();

		// Ped Status
		if (!PedSpawned)
			ImGui::BeginDisabled();

		ImGui::Text("Entity Index: %u (0x%X)", PedDebug.ent, PedDebug.ent);
		ImGui::Text("Coords: %.2f, %.2f, %.2f", PedDebug.pos.x, PedDebug.pos.y, PedDebug.pos.z);
		ImGui::Text("Heading: %.2f", PedDebug.head);
		ImGui::Text("Health: %d/%d", PedDebug.health, PedDebug.max_health);
		ImGui::Text("Injured: %s", (PedDebug.injured ? "true" : "false"));

		if (!PedSpawned)
			ImGui::EndDisabled();

		ImGui::EndGroup();
	}

	void RenderTextDebug()
	{
		TRY
		{
			ImGui::Text("Text Debug");
			ImGui::InputText("Text Input", TextBuffer, IM_ARRAYSIZE(TextBuffer));

			ImGui::PushItemWidth(200);
			ImGui::InputInt("Text Flags", &TextFlags, 2);
			ImGui::PopItemWidth();
			ImGui::SameLine();

			ImGui::Checkbox("Enable Text", &EnableTextDebug);
			ImGui::Separator();

			ImGui::PushItemWidth(200);
			ImGui::SliderFloat("X Position", &TextDebugX, 0, 1);
			ImGui::SameLine();
			ImGui::SliderFloat("Y Position", &TextDebugY, 0, 1);

			ImGui::SliderFloat("Scale", &TextDebugScale, 0, 5);
			ImGui::PopItemWidth();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	
	void RenderHUDContextDebug()
	{
		static int CurCtx = 0;
		static constexpr char* HUDContextList[]{
			"HUD_CTX_CODE_TOOLS",
			"HUD_CTX_CRAFTING_SEQUENCE",
			"HUD_CTX_FADED_GAMEPLAY",
			"HUD_CTX_FIREFIGHT_CUTSCENE",
			"HUD_CTX_FISHING",
			"HUD_CTX_GOLD_CURRENCY_CHANGE",
			"HUD_CTX_HACK_RADAR_FORCE_HIDE",
			"HUD_CTX_HONOR_SHOW",
			"HUD_CTX_INFINITE_AMMO",
			"HUD_CTX_INFO_CARD",
			"HUD_CTX_INPUT_REVEAL_HUD",
			"HUD_CTX_INSPECT_ITEM",
			"HUD_CTX_IN_CAMP_WARDROBE",
			"HUD_CTX_IN_CAMP_WITH_SUPPLIES",
			"HUD_CTX_IN_CATALOGUE_SHOP_MENU",
			"HUD_CTX_IN_COMBAT_RESTRICTED_SHOP",
			"HUD_CTX_IN_FAST_TRAVEL_MENU",
			"HUD_CTX_IN_GUARMA_AND_BROKE",
			"HUD_CTX_IN_LOBBY",
			"HUD_CTX_IN_MINIGAME",
			"HUD_CTX_IN_MINIGAME_POKER_INTRO",
			"HUD_CTX_IN_MINIGAME_POKER_OUTRO",
			"HUD_CTX_IN_MISSION_CUTSCENE",
			"HUD_CTX_IN_MP_GAME_MODE",
			"HUD_CTX_IN_MP_TUTORIAL_CUTSCENE",
			"HUD_CTX_IN_PLAYER_CAMP",
			"HUD_CTX_IN_QUICK_TIME_EVENT",
			"HUD_CTX_IN_RESPAWN",
			"HUD_CTX_IN_SHOP",
			"HUD_CTX_ITEM_CONSUMPTION_DEADEYE",
			"HUD_CTX_ITEM_CONSUMPTION_DEADEYE_CORE",
			"HUD_CTX_ITEM_CONSUMPTION_HEALTH",
			"HUD_CTX_ITEM_CONSUMPTION_HEALTH_CORE",
			"HUD_CTX_ITEM_CONSUMPTION_HORSE_HEALTH",
			"HUD_CTX_ITEM_CONSUMPTION_HORSE_HEALTH_CORE",
			"HUD_CTX_ITEM_CONSUMPTION_HORSE_STAMINA",
			"HUD_CTX_ITEM_CONSUMPTION_HORSE_STAMINA_CORE",
			"HUD_CTX_ITEM_CONSUMPTION_STAMINA",
			"HUD_CTX_ITEM_CONSUMPTION_STAMINA_CORE",
			"HUD_CTX_LOBBY_TEAM_SELECT",
			"HUD_CTX_MAYOR2_CUTSCENE_OBJECTIVES",
			"HUD_CTX_MINIGAME_SHOOTING",
			"HUD_CTX_MISSION_CONTROLLER",
			"HUD_CTX_MISSION_CONTROLLER_CUTSCENE",
			"HUD_CTX_MISSION_CONTROLLER_INTRO",
			"HUD_CTX_MISSION_CONTROLLER_OUTRO",
			"HUD_CTX_MISSION_CREATOR",
			"HUD_CTX_MISSION_CUTSCENE_WITH_RADAR",
			"HUD_CTX_MP_MATCHMAKING_TRANSITION",
			"HUD_CTX_MP_OUT_OF_AREA_BOUNDS",
			"HUD_CTX_MP_RACES",
			"HUD_CTX_MP_SPECTATING",
			"HUD_CTX_NO_ALIVE_PLAYER_HORSE",
			"HUD_CTX_PLAYER_CAMERA_MODE",
			"HUD_CTX_PROMPT_MONEY",
			"HUD_CTX_RESTING_BY_FIRE",
			"HUD_CTX_ROBBERY_ACTION",
			"HUD_CTX_SCRIPTED_IN_GAME_CUTSCENE",
			"HUD_CTX_SCRIPTED_PLAYER_CONTROL_DISABLED",
			"HUD_CTX_SCRIPT_CME_CUTSCENE",
			"HUD_CTX_SHARP_SHOOTER_EVENT",
			"HUD_CTX_SHOP_OBJECTIVE",
			"HUD_CTX_SKINNING_PROCESS",
			"HUD_CTX_TITHING",
			"HUD_CTX_TITHING_NOGANG_CASH",
			"HUD_CTX_TRANSLATE_OVERLAY",
		};

		if (ImGui::BeginCombo("HUD Context", HUDContextList[CurCtx]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(HUDContextList); i++)
			{
				if (ImGui::Selectable(HUDContextList[i], i == CurCtx))
					CurCtx = i;
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("Enable"))
		{
			QUEUE_JOB(=)
			{
				HUD::_ENABLE_HUD_CONTEXT(joaat(HUDContextList[CurCtx]));
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Disable"))
		{
			QUEUE_JOB(=)
			{
				HUD::_DISABLE_HUD_CONTEXT(joaat(HUDContextList[CurCtx]));
			}
			END_JOB()
		}
	}
	
	void RenderCPedDebug()
	{
		static CPed* CurrentPed{};
		static Entity CurrentPedIndex{};

		bool Update = false;

		ImGui::PushItemWidth(200);
		if (ImGui::InputInt("Input Ped Index", &CurrentPedIndex))
			Update = true;
		ImGui::PopItemWidth();

		if (ImGui::Button("Set Local Ped"))
		{
			CurrentPedIndex = g_LocalPlayer.m_Entity;
			Update = true;
		}

		if (Update)
			CurrentPed = Pointers::GetEntityPedReal(CurrentPedIndex);

		ImGui::Text("Entity Index: %u (0x%X)", CurrentPedIndex, CurrentPedIndex);
		ImGui::Text("CPed Address: 0x%llX", (uintptr_t)CurrentPed);
		ImGui::Text("GetEntityPedReal: 0x%llX", (uintptr_t)Pointers::GetEntityPedReal);
	}
	
	void RenderPersCharDebug()
	{
		static PersChar s_PersChar{};
		static Ped s_Ped{};

		if (ImGui::InputInt("Input PersChar", &s_PersChar))
		{
			QUEUE_JOB(&)
			{
				s_Ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(s_PersChar);
			}
			END_JOB()
		}

		ImGui::Text("PersChar: %d", s_PersChar);
		ImGui::Text("Ped: %d", s_Ped);
		ImGui::Text("Model: %s", Features::GetPedModelName(s_Ped).c_str());
	}
	
	void RenderExplosionDebug()
	{
		static const char* s_ExplosionTypes[]{
			"EXP_TAG_DONTCARE",
			"EXP_TAG_GRENADE",
			"EXP_TAG_STICKYBOMB",
			"EXP_TAG_MOLOTOV",
			"EXP_TAG_MOLOTOV_VOLATILE",
			"EXP_TAG_HI_OCTANE",
			"EXP_TAG_CAR",
			"EXP_TAG_PLANE",
			"EXP_TAG_PETROL_PUMP",
			"EXP_TAG_DIR_STEAM",
			"EXP_TAG_DIR_FLAME",
			"EXP_TAG_DIR_WATER_HYDRANT",
			"EXP_TAG_BOAT",
			"EXP_TAG_BULLET",
			"EXP_TAG_SMOKEGRENADE",
			"EXP_TAG_BZGAS",
			"EXP_TAG_GAS_CANISTER",
			"EXP_TAG_EXTINGUISHER",
			"EXP_TAG_TRAIN",
			"EXP_TAG_DIR_FLAME_EXPLODE",
			"EXP_TAG_VEHICLE_BULLET",
			"EXP_TAG_BIRD_CRAP",
			"EXP_TAG_FIREWORK",
			"EXP_TAG_TORPEDO",
			"EXP_TAG_TORPEDO_UNDERWATER",
			"EXP_TAG_LANTERN",
			"EXP_TAG_DYNAMITE",
			"EXP_TAG_DYNAMITESTACK",
			"EXP_TAG_DYNAMITE_VOLATILE",
			"EXP_TAG_RIVER_BLAST",
			"EXP_TAG_PLACED_DYNAMITE",
			"EXP_TAG_FIRE_ARROW",
			"EXP_TAG_DYNAMITE_ARROW",
			"EXP_TAG_PHOSPHOROUS_BULLET",
			"EXP_TAG_LIGHTNING_STRIKE",
			"EXP_TAG_TRACKING_ARROW",
			"EXP_TAG_POISON_BOTTLE",
		};
		static int s_CurrentExplosion = -1;

		if (ImGui::BeginCombo("Explosion Type", s_ExplosionTypes[s_CurrentExplosion + 1]))
		{
			for (int i = 0; i < ARRAYSIZE(s_ExplosionTypes); i++)
			{
				if (ImGui::Selectable(s_ExplosionTypes[i], (i - 1) == s_CurrentExplosion))
					s_CurrentExplosion = i - 1;
			}
			ImGui::EndCombo();
		}

		ImGui::PushButtonRepeat(true);
		if (ImGui::Button("Explode"))
		{
			QUEUE_JOB(=)
			{
				const Vector3 pos = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);
				FIRE::ADD_EXPLOSION(pos.x, pos.y, pos.z, s_CurrentExplosion, 1.0f, true, false, 1.0f);
			}
			END_JOB()
		}
		ImGui::PopButtonRepeat();
	}
}
