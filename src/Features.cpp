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
	void ExecuteAsThread(joaat_t ScriptHash, void(*Function)())
	{
		TRY
		{
			assert(Function && Pointers::ThreadCollection && Pointers::ActiveThread);

			for (uint16_t i = 0; i < Pointers::ThreadCollection->m_pCount; i++)
			{
				GtaThread* thread = Pointers::ThreadCollection->m_pData[i];
				if (!thread || !thread->m_ThreadID || thread->m_ScriptHash != ScriptHash)
					continue;
				
				// Backup and change thread
				scrThread* og_thread = *Pointers::ActiveThread;
				*Pointers::ActiveThread = thread;

				// Execute
				Function();

				// Restore thread 
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
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void OnTick()
	{
		TRY
		{
			// Disable inputs if menu is open
			if (Menu::IsOpen)
				PAD::DISABLE_ALL_CONTROL_ACTIONS(0);

			// Update local player info struct
			GetLocalPlayerInfo();

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
				Features::CleanPed(g_LocalPlayer.m_Entity);

			if (g_Settings["rgb_elec_lantern"].get<bool>())
				RGBElectricLantern();

			if (g_Settings["no_black_borders"].get<bool>())
			{
				CAM::_REQUEST_LETTER_BOX_OVERTIME(-1, -1, false, 17, true, false);
				CAM::_FORCE_LETTER_BOX_THIS_UPDATE();
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

	void RGBTick()
	{
		static int iC = 1, dC = 0;
		// Change increasing color (iC) and decreasing color (dC)
		if (g_rgb[iC] == 255)
		{
			iC++;
			dC++;
			if (iC == 3)
				iC = 0;
			else if (dC == 3)
				dC = 0;
		}
		// Increase and decrease values
		g_rgb[iC]++;
		g_rgb[dC]--;
	}
}
