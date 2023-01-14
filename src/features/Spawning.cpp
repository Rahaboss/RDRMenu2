#include "pch.h"
#include "Spawning.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "JobQueue.h"
#include "Lists.h"

namespace Features
{
	void DeletePed(Ped Handle)
	{
		PED::DELETE_PED(&Handle);
	}

	void EndSpawnPed(Ped Handle)
	{
		Hash Model = ENTITY::GET_ENTITY_MODEL(Handle);
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&Handle);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);
	}

	void EndSpawnVehicle(Hash model, Vehicle Handle)
	{
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&Handle);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::string GetPedModelName(Hash hash)
	{
		auto it = g_PedModelNameList.find(hash);
		if (it != g_PedModelNameList.end())
			return it->second;

		return "Unknown";
	}

	std::string GetVehicleModelName(Hash hash)
	{
		auto it = g_VehicleModelNameList.find(hash);
		if (it != g_VehicleModelNameList.end())
			return it->second;
		
		return "Unknown";
	}

	bool RequestModel(Hash model)
	{
		TRY
		{
			if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_VALID(model))
			{
				printf("%s: 0x%llX is not a valid model hash!\n", __FUNCTION__, (uint64_t)model);
				return false;
			}

			constexpr int MaxRequests = 100;
			for (int i = 0; i < MaxRequests; i++)
			{
				STREAMING::REQUEST_MODEL(model, false);
				YieldThread();
				if (STREAMING::HAS_MODEL_LOADED(model))
					return true;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return false;
	}

	Ped SpawnPed(Hash model)
	{
		Ped ped = 0;
		
		if (!RequestModel(model))
		{
			printf("%s: Couldn't request ped %s!\n", __FUNCTION__, GetPedModelName(model).c_str());
			return 0;
		}

		Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);
		
		for (int i = 0; !ped && i < 10; i++)
		{
			ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z,
				ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity), false, false, false, false);
			YieldThread();
		}

		if (!ped)
		{
			printf("%s: Couldn't spawn ped %s!\n", __FUNCTION__, GetPedModelName(model).c_str());
			return 0;
		}

		ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(ped, true);

		PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true);
		PED::SET_PED_CAN_BE_TARGETTED(ped, true);
		PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ped, g_LocalPlayer.m_Index, true);
		ENTITY::SET_ENTITY_INVINCIBLE(ped, false);
		ENTITY::SET_ENTITY_CAN_BE_DAMAGED(ped, true);
		PED::SET_PED_MODEL_IS_SUPPRESSED(ped, false);

		return ped;
	}

	Pickup SpawnPickup(Hash PickupHash)
	{
		Pickup obj = 0;

		TRY
		{
			const Vector3& Pos = g_LocalPlayer.m_Pos;
			obj = OBJECT::CREATE_PICKUP(PickupHash, Pos.x, Pos.y, Pos.z, 0, 0, false, 0, 0, 0.0f, 0);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return obj;
	}

	Vehicle SpawnVehicle(Hash model, bool warp_into)
	{
		Vehicle veh = 0;

		//TRY
		{
			if (!RequestModel(model))
			{
				printf("%s: Couldn't request vehicle %s!\n", __FUNCTION__, GetPedModelName(model).c_str());
				return 0;
			}

			const Vector3& coords = g_LocalPlayer.m_Pos;
			veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z,
				ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity), false, false, false, false);

			if (!veh)
			{
				printf("%s: Couldn't spawn vehicle %s!\n", __FUNCTION__, GetPedModelName(model).c_str());
				return veh;
			}

			YieldThread();

			DECORATOR::DECOR_SET_BOOL(veh, "wagon_block_honor", true);
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
			if (warp_into)
				PED::SET_PED_INTO_VEHICLE(g_LocalPlayer.m_Entity, veh, -1);

			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		}
		//EXCEPT{ LOG_EXCEPTION(); }
		
		return veh;
	}
}
