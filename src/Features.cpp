#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "Fiber.h"
#include "JobQueue.h"
#include "rage/natives.h"
#include "PlayerInfo.h"
#include "rage/lists.h"
#include "menu/Menu.h"
#include "Renderer.h"

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
				auto thread = Pointers::ThreadCollection->m_pData[i];
				if (!thread || !thread->m_ThreadID || thread->m_ScriptHash != script_hash)
					continue;
				
				// backup and change thread
				auto og_thread = *Pointers::ActiveThread;
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
			//std::cout << "Thread Collection: " << LOG_HEX(Pointers::ThreadCollection) << '\n';
			//std::cout << "Hash of current script: " << SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() << '\n';
			//std::cout << "Player name: " << PLAYER::GET_PLAYER_NAME(0) << '\n';
			//SetSnowType(XmasSecondary);
			//UnlockSPPreorderBonus();
			//PrintNativeHandlerAddress(0x214651FB1DFEBA89);
			//PrintNativeHandlerAddress(0xAF35D0D2583051B0);
			//PrintNativeHandlerAddress(0xB980061DA992779D);
			PrintNativeHandlerAddress(0xFA925AC00EB830B9);
			std::cout << "Coords: " << ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0, TRUE) << ".\n";
			std::cout << "RDR2.exe: " << LOG_HEX(g_BaseAddress) << ".\n";
			std::cout << "CPed: " << LOG_HEX(Pointers::GetPlayerPed(0)) << " (vtbl: " << LOG_HEX(*(void**)Pointers::GetPlayerPed(0)) << ").\n";
			std::cout << "Ped Index: " << LOG_HEX(PLAYER::PLAYER_PED_ID()) << ".\n";
			//std::cout << "DEBUG::GET_GAME_VERSION_NAME: " << DEBUG::GET_GAME_VERSION_NAME() << ".\n";
			std::cout << "CPedFactory: " << LOG_HEX(GetPedFactory()) << " (vtbl: " << LOG_HEX(*(void**)GetPedFactory()) << ").\n";
			std::cout << "Blip Collection: " << LOG_HEX(GetBlipCollection()) << ".\n";
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
			//Menu::RenderMenu();

			NoSliding();
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

			if (EnableGodMode)
				SetGodmode(true);

#if 0
			if (IsKeyHeld(VK_LSHIFT))
			{
				// Shift + Page Up: TP through door
				if (IsKeyClicked(VK_PRIOR /*Page Up*/))
				{
					TeleportThroughDoor();
				}

				// Shift + Page Down: Reveal map
				if (IsKeyClicked(VK_NEXT /*Page Down*/))
				{
					RevealMap();
				}

				// Shift + Delete: Toggle godmode
				if (IsKeyClicked(VK_DELETE))
				{
					TOGGLE_AND_LOG_BOOL(EnableGodMode);
					if (!EnableGodMode)
						SetGodmode(false);
				}

				// Shift + F9: Give core XP items
				if (IsKeyClicked(VK_F9))
				{
					//GiveAgedPirateRum();
					//GiveGinsengElixir();
					//GiveValerianRoot();
					//GiveSingleInventoryItem(CONSUMABLE_HAYCUBE);
					GiveCivilWarHat();
				}

				// Shift + F11: Spawn enemy for good honor
				if (IsKeyClicked(VK_F11))
				{
					SpawnGoodHonorEnemy();
					//SpawnBadHonorEnemy();
				}

				// Shift + F12: Give weapons
				if (IsKeyClicked(VK_F12))
				{
					GiveAllWeapons();
					GiveAllDualWieldWeapons();
					GiveAllAmmo();
					//GiveBackWeapon(WEAPON_SNIPERRIFLE_CARCANO);
					//GiveShoulderWeapon(WEAPON_REPEATER_WINCHESTER);
					//GiveLeftHandWeapon(WEAPON_PISTOL_VOLCANIC);
					//GiveRightHandWeapon(WEAPON_REVOLVER_LEMAT);
				}
			}
			else
			{
				// Page Up: TP to waypoint
				if (IsKeyClicked(VK_PRIOR /*Page Up*/))
				{
					TeleportToWaypoint();
				}

				// Page Down: Clear wanted, restore cores
				if (IsKeyClicked(VK_NEXT /*Page Down*/))
				{
					ClearWanted();
					RestorePlayerCores();
					RestoreHorseCores();
					//GiveGoldCores(g_LocalPlayer.m_Entity);
					//GiveGoldCores(g_LocalPlayer.m_Mount);
					//AddMoney(100000);
					//GiveLeftHandWeapon(WEAPON_PISTOL_M1899);
					//GiveRightHandWeapon(WEAPON_PISTOL_M1899);
					//GiveBackWeapon(WEAPON_SNIPERRIFLE_CARCANO);
					//GiveShoulderWeapon(WEAPON_REPEATER_HENRY);
					//GiveCivilWarHat();
				}

				// Delete: Spawn turret
				if (IsKeyClicked(VK_DELETE))
				{
					// U_M_M_CIRCUSWAGON_01 - 2 head skeleton
					// RE_RALLYDISPUTE_MALES_01 - KKK
					// RE_RALLYSETUP_MALES_01 - KKK
					// RE_RALLY_MALES_01 - KKK leader
					// U_F_M_RHDNUDEWOMAN_01 - xd
					// RE_NAKEDSWIMMER_MALES_01 - xd vol2
					// CS_CRACKPOTROBOT - robot
					Ped ped = SpawnPed(RAGE_JOAAT("S_M_Y_Army_01"));
					
					// BREACH_CANNON
					// GATLING_GUN
					// GATLINGMAXIM02
					// HOTCHKISS_CANNON
					//Vehicle veh = SpawnVehicle(GATLING_GUN);

					//Cam c = CAM::GET_RENDERING_CAM();
					//Vector3 pos = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, 1, 1);
					//CAM::SET_CAM_COORD(c, pos.x, pos.y, pos.z);
				}

				// F8: Give money
				if (IsKeyClicked(VK_F8))
				{
					SetMoney(10000000);
				}

				// F9: Drop current weapon
				if (IsKeyClicked(VK_F9))
				{
					DropCurrentWeapon();
					//SetMoney(10000000);
				}

				// F11: TP to guarma
				if (IsKeyClicked(VK_F11))
				{
					Teleport(1424.31f, -7325.1f, 81.4575f);
				}

				// F12: Toggle no snipers, no reload, no sliding
				if (IsKeyClicked(VK_F12))
				{
					TOGGLE_AND_LOG_BOOL(EnableNoSnipers);
					TOGGLE_AND_LOG_BOOL(EnableNoReload);
					TOGGLE_AND_LOG_BOOL(EnableNoSliding);
					TOGGLE_AND_LOG_BOOL(EnableAddInventoryItemLogging);

					// Teleport(-2798.41f, -4262.28f, -17.5096f); // Mexico tunnel
					// Teleport(-2134.6f, -3430.15f, 33.6615f); // Mexico Nuevo Paraiso
					// SpawnLegendaryAnimal(RAGE_JOAAT("A_C_PANTHER_01"), RAGE_JOAAT("PANTHER_LEGENDARY"), 0x42CD3A6B);
				}
			}
#endif
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RunJobQueue()
	{
		g_JobQueue.Run();
	}

	int FpsTick()
	{
		int result = 0;

		static auto fc_begin = *Pointers::FrameCount;
		static auto time_begin = GetTickCount64();

		auto time_new = GetTickCount64();
		if (time_begin + 1000 < time_new)
		{
			auto fc_new = *Pointers::FrameCount;
			result = fc_new - fc_begin;

			fc_begin = *Pointers::FrameCount;
			time_begin = time_new;
		}

		return result;
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
