#include "pch.h"
#include "Player.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "JobQueue.h"
#include "ScriptGlobal.h"

namespace Features
{
	void AddMoney(int AmountCents)
	{
		MONEY::_MONEY_INCREMENT_CASH_BALANCE(AmountCents, ADD_REASON_DEFAULT);
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
			g_LocalPlayer.m_LastMount = PLAYER::_GET_ACTIVE_HORSE_FOR_PLAYER(g_LocalPlayer.m_Index);
			g_LocalPlayer.m_Vehicle = PED::GET_VEHICLE_PED_IS_IN(g_LocalPlayer.m_Entity, false);
			g_LocalPlayer.m_Pos = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, true, true);
			g_LocalPlayer.m_Ped = CPedFactory::GetLocalPed();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	Entity GetMainEntity()
	{
		if (auto e = GetMountOrVehicle())
			return e;
		return g_LocalPlayer.m_Entity;
	}

	int GetMoney()
	{
		return MONEY::_MONEY_GET_CASH_BALANCE();
	}

	Entity GetMountOrVehicle()
	{
		if (g_LocalPlayer.m_Mount)
			return g_LocalPlayer.m_Mount;
		return g_LocalPlayer.m_Vehicle;
	}

	void NoSliding()
	{
		// PCF_0x435F091E = set ped can run into steep slope
		PED::SET_PED_RESET_FLAG(g_LocalPlayer.m_Entity, PCF_0x435F091E, true);
		if (g_LocalPlayer.m_Mount)
			PED::SET_PED_RESET_FLAG(g_LocalPlayer.m_Mount, PCF_0x435F091E, true);
	}

	bool LoadGround(float x, float y, float z)
	{
		return LoadGround(Vector3(x, y, z));
	}

	bool LoadGround(Vector3 location)
	{
		float groundZ;
		const uint8_t attempts = 10;

		for (uint8_t i = 0; i < attempts; i++)
		{
			// Only request a collision after the first try failed because the location might already be loaded on first attempt.
			for (uint16_t z = 0; i && z < 1000; z += 100)
			{
				STREAMING::REQUEST_COLLISION_AT_COORD(location.x, location.y, (float)z);

				YieldThread();
			}

			if (MISC::GET_GROUND_Z_FOR_3D_COORD(location.x, location.y, 1000.f, &groundZ, false))
			{
				location.z = groundZ + 1.f;

				return true;
			}

			YieldThread();
		}

		return false;
	}

	void RemoveMoney(int AmountCents)
	{
		MONEY::_MONEY_DECREMENT_CASH_BALANCE(AmountCents);
	}

	void ResetPlayerModel()
	{
		QUEUE_JOB(=)
		{
			int* Global_1946054_f_1 = ScriptGlobal(1946054).At(1).Get<int*>();
			if (!Global_1946054_f_1)
				return;

			// medium_update.c: func_392
			switch (*Global_1946054_f_1)
			{
			case -2125499975:
				SetPlayerModel(PLAYER_ZERO);
				break;
			case 1160113249:
				SetPlayerModel(PLAYER_THREE);
				break;
			default:
				printf("%s: Unknown default player model: %d!\n", __FUNCTION__, *Global_1946054_f_1);
				break;
			}
		}
		END_JOB()
	}

	void RestoreHorseCores()
	{
		QUEUE_JOB(=)
		{
			if (!g_LocalPlayer.m_Mount)
				return;

			for (int i = 0; i < 3; i++)
				ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(g_LocalPlayer.m_Mount, i, 100);

			ENTITY::SET_ENTITY_HEALTH(g_LocalPlayer.m_Mount, ENTITY::GET_ENTITY_MAX_HEALTH(g_LocalPlayer.m_Mount, false), false);
			PED::CLEAR_PED_WETNESS(g_LocalPlayer.m_Entity);
		}
		END_JOB()
	}

	void RestorePlayerCores()
	{
		QUEUE_JOB(=)
		{
			for (int i = 0; i < 3; i++)
				ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(g_LocalPlayer.m_Entity, i, 100);

			ENTITY::SET_ENTITY_HEALTH(g_LocalPlayer.m_Entity, ENTITY::GET_ENTITY_MAX_HEALTH(g_LocalPlayer.m_Entity, false), false);
			PLAYER::RESTORE_PLAYER_STAMINA(g_LocalPlayer.m_Index, 100.0);
			PLAYER::_SPECIAL_ABILITY_START_RESTORE(g_LocalPlayer.m_Index, -1, false);
			PED::CLEAR_PED_WETNESS(g_LocalPlayer.m_Entity);
		}
		END_JOB()
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

	void SetMoney(int AmountCents)
	{
		int amount = AmountCents - GetMoney();
		if (amount > 0)
			AddMoney(amount);
		else if (amount < 0)
			RemoveMoney(-amount);
	}

	void SetPlayerModel(Hash Model)
	{
		QUEUE_JOB(=)
		{
			// Check model validity or if model is already set
			if (!IsModelValid(Model) || ENTITY::GET_ENTITY_MODEL(g_LocalPlayer.m_Entity) == Model)
				return;

			// Request the new model
			RequestModel(Model);

			// Set the model
			ScriptGlobal(40).At(39).Get<Hash&>() = Model; // medium_update.c: Global_40.f_39
			ScriptGlobal(1935630).At(2).Get<Hash&>() = Model; // medium_update.c: Global_1935630.f_2
			PLAYER::SET_PLAYER_MODEL(g_LocalPlayer.m_Index, Model, false);

			// Get the player info again, because a new player model will be created
			GetLocalPlayerInfo();

			// Clean up
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);
			PED::_SET_RANDOM_OUTFIT_VARIATION(g_LocalPlayer.m_Entity, true);
			PED::SET_PED_RANDOM_COMPONENT_VARIATION(g_LocalPlayer.m_Entity, 0);
			SetMaxAttributeValue(g_LocalPlayer.m_Entity, PA_HEALTH);
			SetMaxAttributeValue(g_LocalPlayer.m_Entity, PA_STAMINA);
			SetMaxAttributeValue(g_LocalPlayer.m_Entity, PA_SPECIALABILITY);
			RestorePlayerCores();
			GiveWeapon(WEAPON_UNARMED);
		}
		END_JOB()
	}

	void SetNoRagdoll(bool Toggle)
	{
		if (Toggle)
			PED::SET_RAGDOLL_BLOCKING_FLAGS(g_LocalPlayer.m_Entity, -1);
		else
			PED::CLEAR_RAGDOLL_BLOCKING_FLAGS(g_LocalPlayer.m_Entity, -1);
	}

	void SpawnBadHonorEnemy(Hash Model)
	{
		TRY
		{
			Ped ped = SpawnPed(Model);
			DECORATOR::DECOR_SET_INT(ped, "honor_override", 9999);
			TASK::TASK_COMBAT_PED(ped, g_LocalPlayer.m_Entity, 0, 0);
			EndSpawnPed(ped);
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
			EndSpawnPed(ped);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void SuperRun()
	{
		if (PED::IS_PED_RAGDOLL(g_LocalPlayer.m_Entity))
			return;

		if (TASK::IS_PED_RUNNING(g_LocalPlayer.m_Entity) || TASK::IS_PED_SPRINTING(g_LocalPlayer.m_Entity))
			ENTITY::APPLY_FORCE_TO_ENTITY(g_LocalPlayer.m_Entity, 1, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1, true, true, true, false, true);
	}

	void Teleport(float x, float y, float z)
	{
		LoadGround(x, y, z);
		ENTITY::SET_ENTITY_COORDS(GetMainEntity(), x, y, z, false, false, false, false);
	}

	void Teleport(const Vector3& pos)
	{
		Teleport(pos.x, pos.y, pos.z);
	}

	void TeleportOnGround(float x, float y, float z)
	{
		QUEUE_JOB(=)
		{
			Teleport(x, y, z - 1.0f);
			YieldThread();
			ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(Features::GetMainEntity(), true);
		}
		END_JOB()
	}

	void TeleportOnGround(const Vector3& pos)
	{
		TeleportOnGround(pos.x, pos.y, pos.z);
	}

	void TeleportThroughDoor()
	{
		Teleport(ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3));
	}

	void TeleportToWaypoint()
	{
		QUEUE_JOB(=)
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
				printf("Waypoint not active.\n");
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

				coords.z = (useGroundZ ? groundZ : g_LocalPlayer.m_Pos.z);
			}

			Teleport(coords);
		}
		END_JOB()
	}
}
