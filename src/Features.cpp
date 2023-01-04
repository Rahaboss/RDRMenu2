#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "Renderer.h"
#include "JobQueue.h"
#include "Fiber.h"
#include "PlayerInfo.h"
#include "Lists.h"
#include "Menu.h"

namespace Features
{
	void ExecuteAsThread(joaat_t script_hash, void(*function)())
	{
		TRY
		{
			if (!function || !Pointers::ThreadCollection || !Pointers::ActiveThread)
				return;

			for (uint16_t i = 0; i < Pointers::ThreadCollection->m_pCount; i++)
			{
				GtaThread* thread = Pointers::ThreadCollection->m_pData[i];
				if (!thread || !thread->m_ThreadID || thread->m_ScriptHash != script_hash)
					continue;
				
				// backup and change thread
				scrThread* og_thread = *Pointers::ActiveThread;
				*Pointers::ActiveThread = thread;

				// execute
				function();

				// restore thread 
				*Pointers::ActiveThread = og_thread;
				return;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void Setup()
	{
		TRY
		{
			Lists::Create();

			//std::cout << "Thread Collection: " << LOG_HEX(Pointers::ThreadCollection) << '\n';
			//std::cout << "Hash of current script: " << SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() << '\n';
			//std::cout << "Player name: " << PLAYER::GET_PLAYER_NAME(0) << '\n';
			//SetSnowType(XmasSecondary);
			//UnlockSPPreorderBonus();
			//PrintNativeHandlerAddress(0x214651FB1DFEBA89);
			//PrintNativeHandlerAddress(0xAF35D0D2583051B0);
			//PrintNativeHandlerAddress(0xB980061DA992779D);
			//PrintNativeHandlerAddress(0xFA925AC00EB830B9);
			//std::cout << "Coords: " << ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0, TRUE) << ".\n";
			//std::cout << "RDR2.exe: " << LOG_HEX(g_BaseAddress) << ".\n";
			//std::cout << "CPed: " << LOG_HEX(Pointers::GetPlayerPed(0)) << " (vtbl: " << LOG_HEX(*(void**)Pointers::GetPlayerPed(0)) << ").\n";
			//std::cout << "Ped Index: " << LOG_HEX(PLAYER::PLAYER_PED_ID()) << ".\n";
			//std::cout << "DEBUG::GET_GAME_VERSION_NAME: " << DEBUG::GET_GAME_VERSION_NAME() << ".\n";
			//std::cout << "CPedFactory: " << LOG_HEX(GetPedFactory()) << " (vtbl: " << LOG_HEX(*(void**)GetPedFactory()) << ").\n";
			//std::cout << "Blip Collection: " << LOG_HEX(GetBlipCollection()) << ".\n";
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void OnTick()
	{
		TRY
		{
			if (Renderer::MenuOpen)
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

			GetLocalPlayerInfo();
			RGBTick();

			//RenderTextOnEntity(g_LocalPlayer.m_Entity, "Player ~n~~COLOR_RED~Test \xE2\x88\x91~ws~");
			//RenderTextOnEntity(g_LocalPlayer.m_Mount, "Mount");
			//RenderTextOnEntity(g_LocalPlayer.m_Vehicle, "Vehicle");
			// \xE2\x88\x91 = Rockstar
			// \xC2\xA6		= Verified
			// \xE2\x80\xB9 = Created
			// \xE2\x80\xBA = Blank
			// \xCE\xA9		= Padlock
			//RenderText("~COLOR_SOCIAL_CLUB~\xE2\x88\x91 \\xE2\\x88\\x91\n\xC2\xA6 \\xC2\\xA6\n\xE2\x80\xB9 \\xE2\\x80\\xB9\n"
			//	"\xE2\x80\xBA \\xE2\\x80\\xBA\n\xCE\xA9 \\xCE\\xA9", 0.0f, 0.0f);
			//RenderText("~COLOR_RED~\xE2\x88\x91 Admin", 0.0f, 0.1f);

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

			if (g_Settings["no_black_borders"].get<bool>())
			{
				CAM::_REQUEST_LETTER_BOX_OVERTIME(-1, -1, FALSE, 17, TRUE, FALSE);
				CAM::_FORCE_LETTER_BOX_THIS_UPDATE();
			}

			if (g_Settings["clean_player"].get<bool>())
				Features::CleanPed(g_LocalPlayer.m_Entity);

			if (g_LocalPlayer.m_Mount)
			{
				if (g_Settings["mount_god_mode"].get<bool>())
					ENTITY::SET_ENTITY_INVINCIBLE(g_LocalPlayer.m_Mount, TRUE);

				if (g_Settings["mount_gold_cores"].get<bool>())
					GiveGoldCores(g_LocalPlayer.m_Mount);
			}

			auto& pd = Menu::PedDebug;
			if (pd.ent)
			{
				if (ENTITY::DOES_ENTITY_EXIST(pd.ent))
				{
					pd.pos = ENTITY::GET_ENTITY_COORDS(pd.ent, TRUE, TRUE);
					pd.head = ENTITY::GET_ENTITY_HEADING(pd.ent);
					pd.health = ENTITY::GET_ENTITY_HEALTH(pd.ent);
					pd.max_health = ENTITY::GET_ENTITY_MAX_HEALTH(pd.ent, TRUE);
					pd.injured = PED::IS_PED_INJURED(pd.ent);

					ENTITY::FREEZE_ENTITY_POSITION(pd.ent, pd.freeze);
					if (pd.freeze)
					{
						TASK::CLEAR_PED_TASKS(pd.ent, FALSE, TRUE);
						TASK::CLEAR_PED_SECONDARY_TASK(pd.ent);
						TASK::CLEAR_PED_TASKS_IMMEDIATELY(pd.ent, TRUE, TRUE);
						ENTITY::SET_ENTITY_HEADING(pd.ent, 0);
					}
					ENTITY::SET_ENTITY_INVINCIBLE(pd.ent, pd.invincible);
					ENTITY::SET_ENTITY_VISIBLE(pd.ent, pd.visible);
				}
				else
					pd.ent = 0;
			}

			if (g_Settings["rgb_elec_lantern"].get<bool>())
				RGBElectricLantern();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RunJobQueue()
	{
		g_JobQueue.Run();
	}

	void YieldThread()
	{
		Fiber::GetCurrent()->YieldThread();
	}

	static int iC = 1, dC = 0;
	void RGBTick()
	{
		if (g_rgb[iC] == 255)
		{
			iC++;
			dC++;
			if (iC == 3)
				iC = 0;
			else if (dC == 3)
				dC = 0;
		}
		g_rgb[iC]++;
		g_rgb[dC]--;
	}
}
