#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "PlayerInfo.h"
#include "Lists.h"
#include "Menu.h"
#include "ScriptGlobal.h"

namespace Features
{
	void Setup()
	{
		TRY
		{
			Lists::Create();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void Cleanup()
	{
		TRY
		{
			// Disable freecam
			if (Menu::EnableFreeCam && Menu::CamEntity)
			{
				CAM::SET_CAM_ACTIVE(Menu::CamEntity, false);
				CAM::RENDER_SCRIPT_CAMS(false, true, 500, true, true, 0);
				CAM::DESTROY_CAM(Menu::CamEntity, false);
				STREAMING::SET_FOCUS_ENTITY(g_LocalPlayer.m_Entity);

				ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Vehicle, false);

				Menu::CamEntity = 0;
			}
			
			// Disable godmode
			//SetGodmode(false);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void OnTick()
	{
		TRY
		{
			// Update local player info struct
			GetLocalPlayerInfo();
			
			// Try to prevent weird bugs
			if (!ENTITY::DOES_ENTITY_EXIST(g_LocalPlayer.m_Entity))
				return;

			// Disable inputs if menu is open
			if (Menu::IsOpen)
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

			// Update RGB state
			RGBTick();

			// Tick functions
			if (g_Settings["no_sliding"].get<bool>())
				NoSliding();

			if (g_Settings["god_mode"].get<bool>())
				SetGodmode(true);
			
			if (g_Settings["gold_cores"].get<bool>())
				GiveGoldCores(g_LocalPlayer.m_Entity);

			if (g_Settings["super_jump"].get<bool>())
				MISC::SET_SUPER_JUMP_THIS_FRAME(g_LocalPlayer.m_Index);

			if (g_Settings["never_wanted"].get<bool>())
				ClearWanted();

			if (g_Settings["clean_player"].get<bool>())
				CleanPed(g_LocalPlayer.m_Entity);

			if (g_Settings["super_run"].get<bool>())
				SuperRun();

			if (g_Settings["freeze_player"].get<bool>())
			{
				ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Entity, true);
				TASK::CLEAR_PED_TASKS(g_LocalPlayer.m_Entity, false, true);
				TASK::CLEAR_PED_SECONDARY_TASK(g_LocalPlayer.m_Entity);
				TASK::CLEAR_PED_TASKS_IMMEDIATELY(g_LocalPlayer.m_Entity, true, true);
			}

			if (g_Settings["no_ragdoll"].get<bool>())
				SetNoRagdoll(true);

			if (g_Settings["rgb_elec_lantern"].get<bool>())
				RGBElectricLantern();

			if (g_Settings["no_black_borders"].get<bool>())
			{
				CAM::_REQUEST_LETTER_BOX_OVERTIME(-1, -1, false, 17, true, false);
				CAM::_FORCE_LETTER_BOX_THIS_UPDATE();
			}

			if (g_Settings["render_model_debug_gun"].get<bool>())
			{
				[=](){
					Entity out;
					if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(g_LocalPlayer.m_Index, &out))
					{
						Hash hash = ENTITY::GET_ENTITY_MODEL(out);

						auto it = g_PedModelNameList.find(hash);
						if (it != g_PedModelNameList.end())
							RenderTextOnEntity(out, it->second.c_str());
						else
						{
							auto it2 = g_VehicleModelNameList.find(hash);
							if (it2 != g_VehicleModelNameList.end())
								RenderTextOnEntity(out, it2->second.c_str());
							else
							{
								RenderTextOnEntity(out, std::to_string(hash).c_str());
							}
						}
					}
				}();
			}

			// Local player mount options
			if (g_LocalPlayer.m_Mount && ENTITY::DOES_ENTITY_EXIST(g_LocalPlayer.m_Mount))
			{
				// Tick functions
				if (g_Settings["mount_god_mode"].get<bool>())
					ENTITY::SET_ENTITY_INVINCIBLE(g_LocalPlayer.m_Mount, true);

				if (g_Settings["mount_gold_cores"].get<bool>())
					GiveGoldCores(g_LocalPlayer.m_Mount);
			}

			// Ped Debug
			if (Menu::PedDebug.ent)
			{
				auto& pd = Menu::PedDebug;
				if (ENTITY::DOES_ENTITY_EXIST(pd.ent))
				{
					pd.pos = ENTITY::GET_ENTITY_COORDS(pd.ent, true, true);
					pd.head = ENTITY::GET_ENTITY_HEADING(pd.ent);
					pd.health = ENTITY::GET_ENTITY_HEALTH(pd.ent);
					pd.max_health = ENTITY::GET_ENTITY_MAX_HEALTH(pd.ent, true);
					pd.injured = PED::IS_PED_INJURED(pd.ent);

					ENTITY::FREEZE_ENTITY_POSITION(pd.ent, pd.freeze);
					if (pd.freeze)
					{
						TASK::CLEAR_PED_TASKS(pd.ent, false, true);
						TASK::CLEAR_PED_SECONDARY_TASK(pd.ent);
						TASK::CLEAR_PED_TASKS_IMMEDIATELY(pd.ent, true, true);
						ENTITY::SET_ENTITY_HEADING(pd.ent, 0);
					}
					ENTITY::SET_ENTITY_INVINCIBLE(pd.ent, pd.invincible);
					ENTITY::SET_ENTITY_VISIBLE(pd.ent, pd.visible);
				}
				else
					pd.ent = 0;
			}

			// Freecam
			if (Menu::EnableFreeCam)
			{
				static Vector3 vecPosition;
				static Vector3 vecRot;
				static float speed = 0.5f;
				static float mult = 0.f;

				if (!Menu::CamEntity)
				{
					Menu::CamEntity = CAM::CREATE_CAM("DEFAULT_SCRIPTED_CAMERA", 0);

					vecPosition = CAM::GET_GAMEPLAY_CAM_COORD();
					vecRot = CAM::GET_GAMEPLAY_CAM_ROT(2);

					ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Vehicle, true);
					ENTITY::FREEZE_ENTITY_POSITION(g_LocalPlayer.m_Mount, true);

					CAM::SET_CAM_COORD(Menu::CamEntity, vecPosition.x, vecPosition.y, vecPosition.z);
					CAM::SET_CAM_ROT(Menu::CamEntity, vecRot.x, vecRot.y, vecRot.z, 2);
					CAM::SET_CAM_ACTIVE(Menu::CamEntity, true);
					CAM::RENDER_SCRIPT_CAMS(true, true, 500, true, true, 0);
				}
				
				Vector3 vecChange = { 0.f, 0.f, 0.f };

				// Left Shift
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_JUMP))
					vecChange.z += speed / 2;
				// Left Control
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_SPRINT))
					vecChange.z -= speed / 2;
				// Forward
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_UP_ONLY))
					vecChange.y += speed;
				// Backward
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_DOWN_ONLY))
					vecChange.y -= speed;
				// Left
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_LEFT_ONLY))
					vecChange.x -= speed;
				// Right
				if (PAD::IS_DISABLED_CONTROL_PRESSED(0, INPUT_MOVE_RIGHT_ONLY))
					vecChange.x += speed;

				if (vecChange.x == 0.f && vecChange.y == 0.f && vecChange.z == 0.f)
					mult = 0.f;
				else if (mult < 10)
					mult += 0.15f;

				Vector3 rot = CAM::GET_CAM_ROT(Menu::CamEntity, 2);
				float yaw = DegreeToRadian(rot.z);

				vecPosition.x += (vecChange.x * cos(yaw) - vecChange.y * sin(yaw)) * mult;
				vecPosition.y += (vecChange.x * sin(yaw) + vecChange.y * cos(yaw)) * mult;
				vecPosition.z += vecChange.z * mult;

				CAM::SET_CAM_COORD(Menu::CamEntity, vecPosition.x, vecPosition.y, vecPosition.z);
				STREAMING::SET_FOCUS_POS_AND_VEL(vecPosition.x, vecPosition.y, vecPosition.z, 0.f, 0.f, 0.f);

				vecRot = CAM::GET_GAMEPLAY_CAM_ROT(2);
				CAM::SET_CAM_ROT(Menu::CamEntity, vecRot.x, vecRot.y, vecRot.z, 2);

				if (!Menu::IsOpen)
				{
					static Hash controls[]{
						INPUT_LOOK_LR,
						INPUT_LOOK_UD,
						INPUT_LOOK_UP_ONLY,
						INPUT_LOOK_DOWN_ONLY,
						INPUT_LOOK_LEFT_ONLY,
						INPUT_LOOK_RIGHT_ONLY,
					};

					PAD::DISABLE_ALL_CONTROL_ACTIONS(0);
					for (const auto& c : controls)
					{
						PAD::ENABLE_CONTROL_ACTION(0, c, true);
						PAD::ENABLE_CONTROL_ACTION(1, c, true);
					}

					if (IsKeyClicked(VK_RETURN))
						Teleport(vecPosition);
				}
			}

			// Text Debug
			if (Menu::EnableTextDebug)
			{
				UIDEBUG::_BG_SET_TEXT_SCALE(Menu::TextDebugScale, Menu::TextDebugScale);
				UIDEBUG::_BG_SET_TEXT_COLOR(255, 255, 255, 255);
				auto vs = MISC::VAR_STRING(Menu::TextFlags, "LITERAL_STRING", Menu::TextBuffer);
				UIDEBUG::_BG_DISPLAY_TEXT(vs, Menu::TextDebugX, Menu::TextDebugY);
			}

			if (g_Settings["rapid_fire"].get<bool>())
				RapidFire();

			if (g_Settings["disable_west_elizabeth_pinkertons"].get<bool>())
				if (auto b = ScriptGlobal(1934266).At(56).Get<bool*>())
					*b = true;
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
