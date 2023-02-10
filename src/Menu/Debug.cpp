#include "pch.h"
#include "Debug.h"
#include "PlayerInfo.h"
#include "JobQueue.h"
#include "ScriptGlobal.h"
#include "Signature.h"
#include "Lists.h"
#include "Features.h"
#include "CutsceneHelper.h"

namespace Menu
{
	void RenderDebugTab()
	{
		if (!ImGui::BeginTabItem("Debug"))
			return;
		
		ImGui::BeginChild("debug_child");

		//ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Debug Info"))
			RenderDebugInfo();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Debug Toggles"))
			RenderDebugToggles();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Debug Buttons"))
			RenderDebugButtons();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Coord Debug"))
			RenderCoordDebug();
		ImGui::Separator();

		ImGui::SetNextItemOpen(true, ImGuiCond_Once);
		if (ImGui::CollapsingHeader("Cutscene Debug"))
			RenderCutsceneDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Ped Debug"))
			RenderPedDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("Text Debug"))
			RenderTextDebug();
		ImGui::Separator();

		if (ImGui::CollapsingHeader("HUD Context Debug"))
			RenderHUDContextDebug();

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
				if (g_Settings["disable_default_cutscenes"].get<bool>() && g_Cutscenes[i].size() == 1)
					continue;
				
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
		ImGui::PushItemWidth(300);
		ImGui::InputText("###filter_cut", CutFilter, 200);
		ImGui::PopItemWidth();
		
		ImGui::Checkbox("Disable Default Cutscenes", g_Settings["disable_default_cutscenes"].get<bool*>());
		ImGui::Separator();

		const nlohmann::json& CurrentJson = g_Cutscenes[s_CurrentCutscene];
		if (ImGui::Button("Play"))
			Features::PlayCutsceneFromJson(CurrentJson);

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
		}

		ImGui::EndGroup();
		ImGui::Separator();

		ImGui::Text("Beta / Special Cutscenes");
		if (ImGui::Button("A Test of Faith II"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayDinoLadyCutscene();
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("A Fisher of Fish II"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayFishCollectorCutscene();
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("The Gilded Cage"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayIndustryCutscene();
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Jack Cutscene"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayJackCutscene();
			}
			END_JOB()
		}
		
		if (ImGui::Button("Money Lending and Other Sins"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayDebtCollectorCutscene();
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Annesburg Jail Breakout With Charles"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayAnnesburgBreakoutCutscene();
			}
			END_JOB()
		}

		if (ImGui::Button("The Fine Art of Conversation"))
		{
			QUEUE_JOB(=)
			{
				[=]() {
					int index = 0;
					for (int i = 0; i < g_Cutscenes.size(); i++)
					{
						if (g_Cutscenes[i]["id"].get_ref<const std::string&>().find("cutscene@ntv2_ext") != std::string::npos)
						{
							index = i;
							break;
						}
					}

					LOG_TO_CONSOLE("Playing cutscene %d.\n", index);
					CutsceneHelper cs(g_Cutscenes[index]);
					cs.PlayAutomatically();
				}();
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Charles Leaving To Help Natives"))
		{
			QUEUE_JOB(=)
			{
				Features::PlayCharlesLeavingCutscene();
			}
			END_JOB()
		}
	}

	void RenderDebugButtons()
	{
		if (ImGui::Button("Get Height"))
		{
			QUEUE_JOB(=)
			{
				LOG_TO_CONSOLE("Ped height: %.2f.\n", PED::_GET_PED_HEIGHT(g_LocalPlayer.m_Entity));
			}
			END_JOB();
		}
		ImGui::SameLine();
		if (ImGui::Button("A Quiet Time Cutscene"))
		{
			QUEUE_JOB(=)
			{
				const char* animDict = "cutscene@SAL1_INT";
				auto iLocal_31 = ANIMSCENE::_CREATE_ANIM_SCENE(animDict, 0, 0, false, true);

				//Ped arthur = Features::SpawnPed(PLAYER_ZERO);
				Ped arthur = Features::SpawnPed(PLAYER_THREE);
				ANIMSCENE::SET_ANIM_SCENE_ENTITY(iLocal_31, "ARTHUR", arthur, 0);

				Ped dutch = Features::SpawnPed(CS_DUTCH);
				ANIMSCENE::SET_ANIM_SCENE_ENTITY(iLocal_31, "DUTCH", dutch, 0);

				Ped molly = Features::SpawnPed(CS_MOLLYOSHEA);
				ANIMSCENE::SET_ANIM_SCENE_ENTITY(iLocal_31, "MollyOshea", molly, 0);

				Ped lenny = Features::SpawnPed(CS_LENNY);
				ANIMSCENE::SET_ANIM_SCENE_ENTITY(iLocal_31, "Lenny", lenny, 0);

				for (int i = 0; i < 20 && !ANIMSCENE::IS_ANIM_SCENE_LOADED(iLocal_31, true, false); i++)
				{
					ANIMSCENE::LOAD_ANIM_SCENE(iLocal_31);
					Features::YieldThread();
				}
				ANIMSCENE::START_ANIM_SCENE(iLocal_31);

				while (!ANIMSCENE::IS_ANIM_SCENE_FINISHED(iLocal_31, false))
				{
					Features::YieldThread();
				}

				PED::DELETE_PED(&arthur);
				PED::DELETE_PED(&dutch);
				PED::DELETE_PED(&molly);
				PED::DELETE_PED(&lenny);
			}
			END_JOB()
		}

		if (ImGui::Button("Corrupt scrNativeCallContext::SetVectorResults"))
			*(Signature("8B 41 18 4C 8B C1 85 C0").Get<uint8_t*>()) = 0xC3;

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
	}

	void RenderDebugInfo()
	{
		ImGui::BeginGroup();

		auto Global_35 = ScriptGlobal(35).Get<Ped*>();
		ImGui::Text("Global_35: 0x%llX", Global_35);
		if (Global_35)
			ImGui::Text("Global_35 = %u", *Global_35);
		ImGui::Text("g_LocalPlayer.m_Entity = %u", g_LocalPlayer.m_Entity);
		auto Global_40_f_39 = ScriptGlobal(40).At(39).Get<Hash*>();
		ImGui::Text("Global_40.f_39: 0x%llX", Global_40_f_39);
		if (Global_40_f_39)
		{
			ImGui::Text("Global_40.f_39 = %u", *Global_40_f_39);
			ImGui::Text("Global_40.f_39 = %s", Features::GetPedModelName(*Global_40_f_39));
		}
		if (Pointers::IsSessionStarted)
			ImGui::Text("Is Session Started: %s", *Pointers::IsSessionStarted ? "true" : "false");

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
		if (ImGui::Button("Copy Address"))
			LOG_TO_CLIPBOARD("%llX", Pointers::GetEntityAddress(g_LocalPlayer.m_Entity));
		
		uint64_t nhash = 0xFA925AC00EB830B9; // 0xBD5DD5EAE2B6CE14; // 0xB980061DA992779D; // 0xED40380076A31506; // 0xA86D5F069399F44D; // 0x25ACFC650B65C538;
		auto addr = (uintptr_t)NativeContext::GetHandler(nhash);
		auto off = addr - g_BaseAddress;
		ImGui::AlignTextToFramePadding();
		ImGui::Text("0x%llX handler: RDR2.exe+0x%X", nhash, off);

		if (ImGui::Button("Print to console"))
			LOG_TO_CONSOLE("0x%llX handler: RDR2.exe+0x%llX (0x%llX).\n", nhash, off, (off + 0x7FF73CAB0000 /*imagebase in ida*/));
		ImGui::SameLine();
		if (ImGui::Button("Copy IDA Address"))
			LOG_TO_CLIPBOARD("%llX", off + 0x7FF73CAB0000);

		ImGui::Text("\xE2\x84\xAE \xE2\x84\x85 \xE2\x88\x91 \xE2\x86\x95 \xC6\xB1");
		ImGui::Text("\xE2\x88\x91 \xC2\xA6 \xE2\x80\xB9 \xE2\x80\xBA \xCE\xA9\n");

		ImGui::EndGroup();
	}

	void RenderDebugToggles()
	{
		ImGui::BeginGroup();
		ImGui::Checkbox("Log Ped Spawning", g_Settings["log_ped_spawning"].get<bool*>());
		ImGui::Checkbox("Log Human Spawning", g_Settings["log_human_spawning"].get<bool*>());
		ImGui::Checkbox("Log Vehicle Spawning", g_Settings["log_vehicle_spawning"].get<bool*>());
		ImGui::Checkbox("Log Added Inventory Items", g_Settings["log_added_inventory_items"].get<bool*>());
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
		ImGui::SameLine();

		ImGui::BeginGroup();
		ImGui::Checkbox("Log Created Cutscenes", g_Settings["log_created_cutscenes"].get<bool*>());
		ImGui::Checkbox("Log Set Decor", g_Settings["log_set_decor"].get<bool*>());
		ImGui::Checkbox("Enable ImGui Demo Window", g_Settings["enable_imgui_demo"].get<bool*>());
		ImGui::Checkbox("Enable Overlay", g_Settings["enable_overlay"].get<bool*>());
		ImGui::EndGroup();
	}

	void RenderPedDebug()
	{
		const bool PedSpawned = PedDebug.ent;

		ImGui::PushItemWidth(350.0f);
		ImGui::BeginGroup();

		// Ped controls
		if (ImGui::Button("Spawn"))
		{
			QUEUE_JOB(= )
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
			QUEUE_JOB(= )
			{
				Features::DeletePed(PedDebug.ent);
				Features::EndSpawnPed(PedDebug.ent);
				PedDebug.ent = 0;
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Resurrect"))
		{
			QUEUE_JOB(= )
			{
				PED::RESURRECT_PED(PedDebug.ent);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Revive"))
		{
			QUEUE_JOB(= )
			{
				PED::REVIVE_INJURED_PED(PedDebug.ent);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Random Component Variations"))
		{
			QUEUE_JOB(= )
			{
				PED::SET_PED_RANDOM_COMPONENT_VARIATION(PedDebug.ent, 0);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Random Outfit Variations"))
		{
			QUEUE_JOB(= )
			{
				PED::_SET_RANDOM_OUTFIT_VARIATION(PedDebug.ent, true);
			}
			END_JOB()
		}

		if (ImGui::Button("Set Good Honor"))
		{
			QUEUE_JOB(= )
			{
				DECORATOR::DECOR_SET_INT(PedDebug.ent, "honor_override", -9999);
			}
			END_JOB()
		}
		ImGui::SameLine();
		if (ImGui::Button("Set Bad Honor"))
		{
			QUEUE_JOB(= )
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
			QUEUE_JOB(= )
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

	void RenderHUDContextDebug()
	{
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
}
