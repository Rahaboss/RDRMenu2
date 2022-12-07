#include "pch.h"
#include "Features.h"
#include "PlayerInfo.h"

namespace Features
{
	void AddMoney(const int& amount_cents)
	{
		MONEY::_MONEY_INCREMENT_CASH_BALANCE(amount_cents, ADD_REASON_DEFAULT);
	}

	void ClearWanted()
	{
		LAW::SET_BOUNTY(g_LocalPlayer.m_Index, 0);
		LAW::_SET_BOUNTY_HUNTER_PURSUIT_CLEARED();
		LAW::SET_WANTED_SCORE(g_LocalPlayer.m_Index, 0);
	}

	void FixAttributes(Ped ped)
	{
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, PA_HEALTH, ATTRIBUTE::GET_MAX_ATTRIBUTE_POINTS(ped, PA_HEALTH));
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, PA_STAMINA, ATTRIBUTE::GET_MAX_ATTRIBUTE_POINTS(ped, PA_STAMINA));
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, PA_SPECIALABILITY, ATTRIBUTE::GET_MAX_ATTRIBUTE_POINTS(ped, PA_SPECIALABILITY));

		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_DIRTINESS, 0);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_DIRTINESSHAT, 0);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_DIRTINESSSKIN, 0);

		constexpr int x = 100;
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_HUNGER, x);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_FATIGUED, x);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_INEBRIATED, x);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_POISONED, x);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_BODYHEAT, x);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_BODYWEIGHT, x);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_OVERFED, x);
		ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, SA_SICKNESS, x);
	}

	void FixHorseAttributes(Ped ped)
	{
		SetMaxAttributeValue(ped, PA_COURAGE);
		SetMaxAttributeValue(ped, PA_AGILITY);
		SetMaxAttributeValue(ped, PA_SPEED);
		SetMaxAttributeValue(ped, PA_ACCELERATION);
		SetMaxAttributeValue(ped, PA_BONDING);
		PED::_SET_MOUNT_BONDING_LEVEL(ped, 4);
	}

	void GetLocalPlayerInfo()
	{
		TRY
		{
			g_LocalPlayer.m_Index = PLAYER::PLAYER_ID();
			g_LocalPlayer.m_Entity = PLAYER::GET_PLAYER_PED_SCRIPT_INDEX(g_LocalPlayer.m_Index);
			g_LocalPlayer.m_Mount = PED::GET_MOUNT(g_LocalPlayer.m_Entity);
			g_LocalPlayer.m_Vehicle = PED::GET_VEHICLE_PED_IS_IN(g_LocalPlayer.m_Entity, FALSE);
			g_LocalPlayer.m_Ped = Pointers::GetPlayerPed(g_LocalPlayer.m_Index);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	Entity GetMainEntity()
	{
		if (auto e = GetMountOrVehicle())
			return e;
		return g_LocalPlayer.m_Entity;
	}

	Entity GetMountOrVehicle()
	{
		if (g_LocalPlayer.m_Mount)
			return g_LocalPlayer.m_Mount;
		return g_LocalPlayer.m_Vehicle;
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

	void NoSliding()
	{
		if (!EnableNoSliding)
			return;

		// PCF_0x435F091E = set ped can run into steep slope
		PED::SET_PED_RESET_FLAG(g_LocalPlayer.m_Entity, PCF_0x435F091E, TRUE);
		if (g_LocalPlayer.m_Mount)
			PED::SET_PED_RESET_FLAG(g_LocalPlayer.m_Mount, PCF_0x435F091E, TRUE);
	}

	void PrintPedAttributes(Ped ped)
	{
		constexpr const char* AttributeNames[]{
			"PA_HEALTH", "PA_STAMINA", "PA_SPECIALABILITY", "PA_COURAGE", "PA_AGILITY", "PA_SPEED",
			"PA_ACCELERATION", "PA_BONDING", "SA_HUNGER", "SA_FATIGUED", "SA_INEBRIATED", "SA_POISONED",
			"SA_BODYHEAT", "SA_BODYWEIGHT", "SA_OVERFED", "SA_SICKNESS", "SA_DIRTINESS", "SA_DIRTINESSHAT",
			"MTR_STRENGTH", "MTR_GRIT", "MTR_INSTINCT", "PA_UNRULINESS", "SA_DIRTINESSSKIN",
		};
		static_assert(ARRAYSIZE(AttributeNames) == MAX_ATTRIBUTES);
			
		TRY
		{
			//const Ped ped = g_LocalPlayer.m_Entity;
			if (!ped)
				return;
			
			//std::cout << "Attr,Rank,BaseRank,BonusRank,MaxRank,DefaultRank,DefaultMaxRank,Points,MaxPoints\n";
			for (int i = 0; i < MAX_ATTRIBUTES; i++)
			{
				auto Attr = AttributeNames[i];
				auto Rank = ATTRIBUTE::GET_ATTRIBUTE_RANK(ped, i);
				auto BaseRank = ATTRIBUTE::GET_ATTRIBUTE_BASE_RANK(ped, i);
				auto BonusRank = ATTRIBUTE::GET_ATTRIBUTE_BONUS_RANK(ped, i);
				auto MaxRank = ATTRIBUTE::GET_MAX_ATTRIBUTE_RANK(ped, i);
				auto DefaultRank = ATTRIBUTE::GET_DEFAULT_ATTRIBUTE_RANK(ped, i);
				auto DefaultMaxRank = ATTRIBUTE::GET_DEFAULT_MAX_ATTRIBUTE_RANK(ped, i);
				auto Points = ATTRIBUTE::GET_ATTRIBUTE_POINTS(ped, i);
				auto MaxPoints = ATTRIBUTE::GET_MAX_ATTRIBUTE_POINTS(ped, i);
				//std::cout << Attr << ',' << Rank << ',' << BaseRank << ',' << BonusRank << ',' << MaxRank << ','
				//	<< DefaultRank << ',' << DefaultMaxRank << ',' << Points << ',' << MaxPoints << '\n';
				std::cout << Attr << " (" << Points << "/" << MaxPoints << ")\n";
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RemoveMoney(const int& amount_cents)
	{
		MONEY::_MONEY_DECREMENT_CASH_BALANCE(amount_cents);
	}

	void RestoreHorseCores()
	{
		if (!g_LocalPlayer.m_Mount)
			return;

		for (int i = 0; i < 2; i++)
			ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(g_LocalPlayer.m_Mount, i, 100);

		ENTITY::SET_ENTITY_HEALTH(g_LocalPlayer.m_Mount, ENTITY::GET_ENTITY_MAX_HEALTH(g_LocalPlayer.m_Mount, FALSE), FALSE);
		PED::CLEAR_PED_WETNESS(g_LocalPlayer.m_Entity);
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

	void SetGodmode(bool Toggle)
	{
		if (g_LocalPlayer.m_Entity)
			ENTITY::SET_ENTITY_INVINCIBLE(g_LocalPlayer.m_Entity, static_cast<BOOL>(Toggle));
	}

	void SetMaxAttributeValue(Ped ped, int attr)
	{
		TRY
		{
			ATTRIBUTE::SET_ATTRIBUTE_POINTS(ped, attr, ATTRIBUTE::GET_MAX_ATTRIBUTE_POINTS(ped, attr));
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void SetMoney(const int& amount_cents)
	{
		int amount = amount_cents - MONEY::_MONEY_GET_CASH_BALANCE();
		if (amount > 0)
			AddMoney(amount);
		else if (amount < 0)
			RemoveMoney(amount);
	}

	void SpawnBadHonorEnemy(Hash Model)
	{
		TRY
		{
			Ped ped = SpawnPed(Model);
			DECORATOR::DECOR_SET_INT(ped, "honor_override", 9999);
			TASK::TASK_COMBAT_PED(ped, g_LocalPlayer.m_Entity, 0, 0);
			EndSpawnPed(Model, ped);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void SpawnGoodHonorEnemy(Hash Model)
	{
		TRY
		{
			Ped ped = SpawnPed(Model);
			DECORATOR::DECOR_SET_INT(ped, "honor_override", -9999);
			TASK::TASK_COMBAT_PED(ped, g_LocalPlayer.m_Entity, 0, 0);
			EndSpawnPed(Model, ped);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void Teleport(const float& x, const float& y, const float& z)
	{
		ENTITY::SET_ENTITY_COORDS(GetMainEntity(), x, y, z, FALSE, FALSE, FALSE, FALSE);
	}

	void Teleport(const Vector3& pos)
	{
		Teleport(pos.x, pos.y, pos.z);
	}

	void TeleportThroughDoor()
	{
		Teleport(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3));
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
}
