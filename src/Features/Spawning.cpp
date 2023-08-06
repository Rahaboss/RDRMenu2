#include "pch.h"
#include "Spawning.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "JobQueue.h"
#include "Lists.h"
#include "Menu/Logger.h"

namespace Features
{
	void DeleteObject(Ped Handle)
	{
		OBJECT::DELETE_OBJECT(&Handle);
	}

	void DeletePed(Ped Handle)
	{
		PED::DELETE_PED(&Handle);
	}

	void DeleteVehicle(Vehicle Handle)
	{
		VEHICLE::DELETE_VEHICLE(&Handle);
	}

	std::string GetPedModelName(Hash hash)
	{
		auto it = g_PedModelNameList.find(hash);
		if (it != g_PedModelNameList.end())
			return it->second;

		return std::string();
	}

	std::string GetVehicleModelName(Hash hash)
	{
		auto it = g_VehicleModelNameList.find(hash);
		if (it != g_VehicleModelNameList.end())
			return it->second;
		
		return std::string();
	}

	std::string GetObjectModelName(Hash hash)
	{
		auto it = g_ObjectModelNameList.find(hash);
		if (it != g_ObjectModelNameList.end())
			return it->second;

		return std::string();
	}
	
	bool IsModelValid(Hash Model)
	{
		return STREAMING::IS_MODEL_IN_CDIMAGE(Model) && STREAMING::IS_MODEL_VALID(Model);
	}

	bool RequestModel(Hash Model)
	{
		TRY
		{
			if (!IsModelValid(Model))
			{
				LOG_TO_CONSOLE("%s: 0x%llX is not a valid model hash!\n", __FUNCTION__, (uint64_t)Model);
				return false;
			}

			constexpr int MaxRequests = 100;
			for (int i = 0; i < MaxRequests; i++)
			{
				STREAMING::REQUEST_MODEL(Model, false);
				YieldThread();
				if (STREAMING::HAS_MODEL_LOADED(Model))
					return true;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return false;
	}

	Object SpawnObject(Hash Model)
	{
		if (!RequestModel(Model))
		{
			LOG_TO_CONSOLE("%s: Couldn't request object %s!\n", __FUNCTION__, GetObjectModelName(Model).c_str());
			return 0;
		}

		const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

		const Object obj = OBJECT::CREATE_OBJECT_NO_OFFSET(Model, coords.x, coords.y, coords.z, true, true, false, true);

		if (!obj)
		{
			LOG_TO_CONSOLE("%s: Couldn't spawn object %s!\n", __FUNCTION__, GetObjectModelName(Model).c_str());
			return 0;
		}

		YieldThread();

		ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(obj, true);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);

		return obj;
	}

	Ped SpawnPed(Hash Model)
	{
		if (!RequestModel(Model))
		{
			LOG_TO_CONSOLE("%s: Couldn't request ped %s!\n", __FUNCTION__, GetPedModelName(Model).c_str());
			return 0;
		}

		const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);
		
		const Ped ped = PED::CREATE_PED(Model, coords.x, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity),
			false, false, false, false);
		
		if (!ped)
		{
			LOG_TO_CONSOLE("%s: Couldn't spawn ped %s!\n", __FUNCTION__, GetPedModelName(Model).c_str());
			return 0;
		}
		
		YieldThread();

		PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true);
		YieldThread();
		ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(ped, true);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);
		
		return ped;
	}

	Pickup SpawnPickup(Hash PickupHash)
	{
		const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);
		return OBJECT::CREATE_PICKUP(PickupHash, coords.x, coords.y, coords.z, 0, 0, false, 0, 0, 0.0f, 0);
	}

	Vehicle SpawnVehicle(Hash Model, bool WarpInside)
	{
		if (!RequestModel(Model))
		{
			LOG_TO_CONSOLE("%s: Couldn't request vehicle %s!\n", __FUNCTION__, GetVehicleModelName(Model).c_str());
			return 0;
		}

		const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);
			
		const Vehicle veh = VEHICLE::CREATE_VEHICLE(Model, coords.x, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity),
			false, false, false, false);

		if (!veh)
		{
			LOG_TO_CONSOLE("%s: Couldn't spawn vehicle %s!\n", __FUNCTION__, GetVehicleModelName(Model).c_str());
			return 0;
		}

		YieldThread();

		DECORATOR::DECOR_SET_BOOL(veh, "wagon_block_honor", true);
		VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);

		if (WarpInside)
			PED::SET_PED_INTO_VEHICLE(g_LocalPlayer.m_Entity, veh, -1);
		
		return veh;
	}

	void LogSpawnedEntity(Entity ent)
	{
		if (!ENTITY::DOES_ENTITY_EXIST(ent))
			return;

		Hash Model = ENTITY::GET_ENTITY_MODEL(ent);
		Vector3 Pos = ENTITY::GET_ENTITY_COORDS(ent, TRUE, TRUE);
		std::string Type, Name;

		if (ENTITY::IS_ENTITY_AN_OBJECT(ent)) // Object
		{
			if (!g_Settings["logging"]["spawned_object"].get<bool>())
				return;
			Type = "object";
			Name = GetObjectModelName(Model);
		}
		else if (ENTITY::IS_ENTITY_A_VEHICLE(ent)) // Vehicle
		{
			if (!g_Settings["logging"]["spawned_vehicle"].get<bool>())
				return;
			Type = "vehicle";
			Name = GetVehicleModelName(Model);
		}
		else if (ENTITY::IS_ENTITY_A_PED(ent))
		{
			if (PED::IS_PED_HUMAN(ent)) // Human
			{
				if (!g_Settings["logging"]["spawned_human"].get<bool>())
					return;
				Type = "human";
			}
			else // Ped (animal / other)
			{
				if (!g_Settings["logging"]["spawned_ped"].get<bool>())
					return;
				Type = "ped";
			}
			Name = GetPedModelName(Model);
		}
		else
		{
			LOG_TO_MENU("Creating unknown entity (ID: %u / 0x%X) at %.2f, %.2f, %.2f\n", ent, ent, Pos.x, Pos.y, Pos.z);
			return;
		}

		// Display the model name as unsigned decimal if unknown
		if (Name.empty())
			Name = std::to_string(Model);

		LOG_TO_MENU("Creating %s %s (ID: %u / 0x%X) at %.2f, %.2f, %.2f\n",
			Type.c_str(), Name.c_str(), ent, ent, Pos.x, Pos.y, Pos.z);
	}
	
	std::string GetModelName(Hash hash)
	{
		std::map<Hash, std::string>::iterator it;

		it = g_PedModelNameList.find(hash);
		if (it != g_PedModelNameList.end())
			return it->second;

		it = g_VehicleModelNameList.find(hash);
		if (it != g_VehicleModelNameList.end())
			return it->second;

		it = g_ObjectModelNameList.find(hash);
		if (it != g_ObjectModelNameList.end())
			return it->second;

		return std::string();
	}
}
