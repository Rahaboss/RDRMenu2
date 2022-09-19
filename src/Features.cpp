#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "Fiber.h"
#include "JobQueue.h"
#include "rage/natives.h"
#include "PlayerInfo.h"
#include "rage/lists.h"

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
		//std::cout << "Thread Collection: " << LOG_HEX(Pointers::ThreadCollection) << '\n';
		//std::cout << "Hash of current script: " << SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() << '\n';
		//std::cout << "Player name: " << PLAYER::GET_PLAYER_NAME(0) << '\n';
		//SetSnowType(XmasSecondary);
		//UnlockSPPreorderBonus();
		PrintNativeHandlerAddress(0x29B30D07C3F7873B);
		constexpr Hash x = RAGE_JOAAT("BLIP_STYLE_OBJECTIVE");
		std::cout << "Coords: " << ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), 0, TRUE) << '\n';
		std::cout << "RDR2.exe: " << LOG_HEX(g_base_address) << '\n';
	}

	void OnTick()
	{
		TRY
		{
			GetLocalPlayerInfo();
			//NoSliding();
			
			if (GetAsyncKeyState(VK_PRIOR) & 1)
				TeleportToWaypoint();

			if (GetAsyncKeyState(VK_NEXT) & 1)
			{
				GiveAllWeapons();
				GiveAllAmmo();
				RevealMap();
				RestorePlayerCores();
				ClearWanted();
				GiveGoldCores(g_LocalPlayer.m_Entity);
				GiveGoldCores(g_LocalPlayer.m_Mount);
				AddMoney(100000);
			}

			if (GetAsyncKeyState(VK_DELETE) & 1)
			{
				EnableNoSnipers = !EnableNoSnipers;
				std::cout << "No snipers: " << (EnableNoSnipers ? "enabled" : "disabled") << '\n';
			}

			if (GetAsyncKeyState(VK_F9) & 1)
				ENTITY::SET_ENTITY_INVINCIBLE(g_LocalPlayer.m_Entity, FALSE);

			if (GetAsyncKeyState(VK_F10) & 1)
				ENTITY::SET_ENTITY_INVINCIBLE(g_LocalPlayer.m_Entity, TRUE);

			// tp to guarma
			if (GetAsyncKeyState(VK_F11) & 1)
				Teleport(1424.31f, -7325.1f, 81.4575f);

			if (GetAsyncKeyState(VK_F12) & 1)
			{
				// Teleport(-2798.41f, -4262.28f, -17.5096f); // Mexico tunnel
				// Teleport(-2134.6f, -3430.15f, 33.6615f); // Mexico Nuevo Paraiso
				// SpawnLegendaryAnimal(RAGE_JOAAT("A_C_PANTHER_01"), RAGE_JOAAT("PANTHER_LEGENDARY"), 0x42CD3A6B);
			}
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

	void GetLocalPlayerInfo()
	{
		TRY
		{
			g_LocalPlayer.m_Index = PLAYER::PLAYER_ID();
			g_LocalPlayer.m_Entity = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_LocalPlayer.m_Index);
			g_LocalPlayer.m_Mount = PED::GET_MOUNT(g_LocalPlayer.m_Entity);
			g_LocalPlayer.m_Vehicle = PED::GET_VEHICLE_PED_IS_IN(g_LocalPlayer.m_Entity, FALSE);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	Entity GetMountOrVehicle()
	{
		if (g_LocalPlayer.m_Mount)
			return g_LocalPlayer.m_Mount;
		return g_LocalPlayer.m_Vehicle;
	}

	Entity GetMainEntity()
	{
		if (auto e = GetMountOrVehicle())
			return e;
		return g_LocalPlayer.m_Entity;
	}

	void Teleport(const float& x, const float& y, const float& z)
	{
		ENTITY::SET_ENTITY_COORDS(GetMainEntity(), x, y, z, FALSE, FALSE, FALSE, FALSE);
	}

	void Teleport(const Vector3& pos)
	{
		Teleport(pos.x, pos.y, pos.z);
	}

	void TeleportToWaypoint()
	{
		TRY
		{
			Vector3 coords{};
			bool found = false, playerBlip = false;
			if (MAP::IS_WAYPOINT_ACTIVE())
			{
				coords = MAP::_GET_WAYPOINT_COORDS();
				found = true;
				playerBlip = true;
			}
#if 0
			else
			{
				//for (int i = 0; i < 3; i++)
				{
					Blip blip = 0;// MAP::GET_FIRST_BLIP_INFO_ID(i);
					if (MAP::DOES_BLIP_EXIST(blip))
					{
						coords = MAP::GET_BLIP_COORDS(blip);
						found = true;
						//break;
					}
				}
			}
#endif

			if (!found)
			{
				std::cout << "Waypoint not active!\n";
				return;
			}
			
			if (playerBlip)
			{
				float groundZ;
				bool useGroundZ;
				for (int i = 0; i < 100; i++)
				{
					float testZ = (i * 10.f) - 100.f;

					Teleport(coords.x, coords.y, testZ);
					if (i % 5 == 0)
						YieldThread();

					useGroundZ = MISC::GET_GROUND_Z_FOR_3D_COORD(coords.x, coords.y, testZ, &groundZ, false);
					if (useGroundZ)
						break;
				}

				coords.z = (useGroundZ ? groundZ : ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE).z);
			}

			Teleport(coords);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void GiveGoldCores(const Ped& ped)
	{
		for (int i = 0; i < 3; i++)
		{
			constexpr float overpower_duration = 1000.f; // seconds
			constexpr int core_value = 100; // 0 - 100
			constexpr BOOL sound = FALSE;

			ATTRIBUTE::DISABLE_ATTRIBUTE_OVERPOWER(ped, i);
			ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(ped, i, core_value);
			ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(ped, i, overpower_duration, sound);
			ATTRIBUTE::ENABLE_ATTRIBUTE_OVERPOWER(ped, i, overpower_duration, sound);
		}
	}

	void GiveWeapon(const Hash& weapon_hash)
	{
		WEAPON::GIVE_DELAYED_WEAPON_TO_PED(g_LocalPlayer.m_Entity, weapon_hash, 9999, TRUE, ADD_REASON_DEFAULT);
	}

	void GiveAllWeapons()
	{
		for (const auto& w : g_WeaponList)
			GiveWeapon(w);
	}

	void GiveAllAmmo()
	{
		for (const auto& a : g_AmmoList)
			WEAPON::_ADD_AMMO_TO_PED_BY_TYPE(g_LocalPlayer.m_Entity, a, 9999, ADD_REASON_DEFAULT);
	}

	void ClearWanted()
	{
		LAW::SET_BOUNTY(g_LocalPlayer.m_Index, 0);
		LAW::_SET_BOUNTY_HUNTER_PURSUIT_CLEARED();
		LAW::SET_WANTED_SCORE(g_LocalPlayer.m_Index, 0);
	}

	void RevealMap()
	{
		MAP::SET_MINIMAP_HIDE_FOW(TRUE);
		MAP::_REVEAL_MINIMAP_FOW(0);
	}

	void PrintNativeHandlerAddress(const uint64_t& hash)
	{
		TRY
		{
			auto addr = (uintptr_t)g_NativeContext.GetHandler(hash);
			auto off = addr - g_base_address;
			std::cout << LOG_HEX(hash) << " handler: RDR2.exe+" << LOG_HEX(off) << " (" << LOG_HEX(0x7FF73CAB0000 /*imagebase in ida*/ + off) << ")\n";
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
	
	void AddMoney(const int& amount_cents)
	{
		MONEY::_MONEY_INCREMENT_CASH_BALANCE(amount_cents, ADD_REASON_DEFAULT);
	}
	
	void RemoveMoney(const int& amount_cents)
	{
		MONEY::_MONEY_DECREMENT_CASH_BALANCE(amount_cents);
	}
	
	void SetMoney(const int& amount_cents)
	{
		int amount = amount_cents - MONEY::_MONEY_GET_CASH_BALANCE();
		if (amount > 0)
			AddMoney(amount);
		else if (amount < 0)
			RemoveMoney(amount);
	}

	bool RequestModel(const Hash& model)
	{
		if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_VALID(model))
			return false;

		for (int i = 0; i < 10; i++)
		{
			STREAMING::REQUEST_MODEL(model, FALSE);
			YieldThread();
			if (STREAMING::HAS_MODEL_LOADED(model))
				return true;
		}

		return false;
	}

	void SpawnPed(const Hash& model)
	{
		if (!RequestModel(model))
		{
			std::cout << "Couldn't spawn ped " << model << '\n';
			return;
		}

		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);
		Ped ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);
		YieldThread();

		PED::_SET_RANDOM_OUTFIT_VARIATION(ped, TRUE);

		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&ped);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	void SpawnVehicle(const Hash& model, const bool& warp_into)
	{
		if (!RequestModel(model))
		{
			std::cout << "Couldn't spawn vehicle " << model << '\n';
			return;
		}

		Vector3 coords = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE);
		Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z,
			ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity), FALSE, FALSE, FALSE, FALSE);
		YieldThread();

		DECORATOR::DECOR_SET_BOOL(veh, "wagon_block_honor", TRUE);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
		if (warp_into)
			PED::SET_PED_INTO_VEHICLE(g_LocalPlayer.m_Entity, veh, -1);

		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	void SetSnowType(eSnowCoverageType type)
	{
		GRAPHICS::_SET_SNOW_COVERAGE_TYPE(type);
	}

	void NoSliding()
	{
		// PCF_0x435F091E = set ped can run into steep slope
		PED::SET_PED_RESET_FLAG(g_LocalPlayer.m_Entity, PCF_0x435F091E, TRUE);
		if (g_LocalPlayer.m_Mount)
			PED::SET_PED_RESET_FLAG(g_LocalPlayer.m_Mount, PCF_0x435F091E, TRUE);
	}
	
	//void SpawnLegendaryAnimal(const Hash& model_hash, const Hash& legendary_hash, const Hash& outfit_hash)
	//{
	//	if (!RequestModel(model_hash) /*|| !RequestModel(legendary_hash)*/)
	//		return;
	//
	//	int outfit = PED::_REQUEST_METAPED_OUTFIT(legendary_hash, outfit_hash);
	//	if (!PED::_IS_METAPED_OUTFIT_REQUEST_VALID(outfit))
	//		return;
	//
	//	Vector3 pos = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE);
	//	Ped animal = PED::CREATE_PED(model_hash, pos.x, pos.y, pos.z, 0, 1, 1, 0, 0);
	//	ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(animal, 1);
	//	FLOCK::_SET_ANIMAL_RARITY(animal, 2);
	//	PED::SET_PED_CONFIG_FLAG(animal, PCF_DisableHeadGore, TRUE);
	//	PED::SET_PED_CONFIG_FLAG(animal, PCF_DisableLimbGore, TRUE);
	//	PED::SET_PED_CONFIG_FLAG(animal, PCF_0x71A55282, TRUE);
	//
	//	PED::SET_PED_CONFIG_FLAG(animal, PCF_DisableMelee, TRUE);
	//	
	//	ENTITY::SET_ENTITY_HEALTH(animal, 0, g_LocalPlayer.m_Entity);
	//	
	//	EVENT::SET_DECISION_MAKER(animal, RAGE_JOAAT("EMTPY"));
	//
	//	if (model_hash == RAGE_JOAAT("A_C_ALLIGATOR_02"))
	//	{
	//		PED::_SET_RANDOM_OUTFIT_VARIATION(animal, 1);
	//	}
	//	else
	//	{
	//		PED::_EQUIP_META_PED_OUTFIT(animal, legendary_hash);
	//		PED::_UPDATE_PED_VARIATION(animal, 0, 1, 1, 1, 0);
	//	}
	//}

	void UnlockSPPreorderBonus()
	{
		constexpr Hash unlock_hash = RAGE_JOAAT("SP_GAME_CONTENT_PRE_ORDER");
		UNLOCK::UNLOCK_SET_UNLOCKED(unlock_hash, TRUE);
	}

	void RestorePlayerCores()
	{
		for (int i = 0; i < 3; i++)
			ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(g_LocalPlayer.m_Entity, i, 100);

		ENTITY::SET_ENTITY_HEALTH(g_LocalPlayer.m_Entity, ENTITY::GET_ENTITY_MAX_HEALTH(g_LocalPlayer.m_Entity, FALSE), FALSE);
		PLAYER::RESTORE_PLAYER_STAMINA(g_LocalPlayer.m_Index, 100.0);
		PLAYER::_SPECIAL_ABILITY_START_RESTORE(g_LocalPlayer.m_Index, -1, FALSE);
		PED::CLEAR_PED_WETNESS(g_LocalPlayer.m_Entity);
	}
}
