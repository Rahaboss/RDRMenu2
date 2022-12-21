#include "pch.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "JobQueue.h"
#include "rage/lists.h"

namespace Features
{
	void EndSpawnPed(Hash Model, Ped Handle)
	{
		ENTITY::SET_PED_AS_NO_LONGER_NEEDED(&Handle);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);
	}

	void EndSpawnVehicle(Hash model, Vehicle Handle)
	{
		ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&Handle);
		STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
	}

	std::string_view GetPedModelName(const Hash& hash)
	{
#if ENABLE_LARGE_STACK_ITEMS
		TRY
		{
			auto it = g_PedModelNameList.find(hash);
			if (it != g_PedModelNameList.end())
				return it->second;
		}
		EXCEPT{ LOG_EXCEPTION(); }
#endif 

		return "Unknown"sv;
	}

	std::string_view GetVehicleModelName(const Hash& hash)
	{
#if ENABLE_LARGE_STACK_ITEMS
		TRY
		{
			auto it = g_VehicleModelNameList.find(hash);
			if (it != g_VehicleModelNameList.end())
				return it->second;
		}
		EXCEPT{ LOG_EXCEPTION(); }
#endif 

		return "Unknown"sv;
	}

	bool RequestModel(const Hash& model)
	{
		TRY
		{
			if (!STREAMING::IS_MODEL_IN_CDIMAGE(model) || !STREAMING::IS_MODEL_VALID(model))
			{
				std::cout << __FUNCTION__ << ": " << LOG_HEX(model) << " is not a valid model hash!\n";
				return false;
			}

			constexpr int MaxRequests = 100;
			for (int i = 0; i < MaxRequests; i++)
			{
				STREAMING::REQUEST_MODEL(model, FALSE);
				YieldThread();
				if (STREAMING::HAS_MODEL_LOADED(model))
					return true;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return false;
	}

	//void SpawnLegendaryAnimal(const Hash& model_hash, const Hash& legendary_hash, const Hash& outfit_hash)
	//{
	//	if (!RequestModel(model_hash))
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

	Ped SpawnPed(const Hash& model)
	{
		Ped ped = 0;
		
		TRY
		{
			if (!RequestModel(model))
			{
				std::cout << __FUNCTION__ << ": Couldn't spawn ped " << LOG_HEX(model) << '\n';
				return 0;
			}

			Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);
			ped = PED::CREATE_PED(model, coords.x, coords.y, coords.z, static_cast<float>(rand() % 360), 0, 0, 0, 0);

			if (!ped)
			{
				std::cout << __FUNCTION__ << ": Couldn't spawn ped " << LOG_HEX(model) << '\n';
				return ped;
			}

			YieldThread();

			PED::_SET_RANDOM_OUTFIT_VARIATION(ped, TRUE);
			PED::SET_PED_CAN_BE_TARGETTED(ped, TRUE);
			PED::SET_PED_CAN_BE_TARGETTED_BY_PLAYER(ped, g_LocalPlayer.m_Index, TRUE);
			ENTITY::SET_ENTITY_INVINCIBLE(ped, FALSE);
			ENTITY::SET_ENTITY_CAN_BE_DAMAGED(ped, TRUE);
			PED::SET_PED_MODEL_IS_SUPPRESSED(ped, FALSE);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return ped;
	}

	Pickup SpawnPickup(Hash PickupHash)
	{
		Pickup obj = 0;

		TRY
		{
			Vector3 Pos = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE);
			obj = OBJECT::CREATE_PICKUP(PickupHash, Pos.x, Pos.y, Pos.z, 0, 0, FALSE, 0, 0, 0.0f, 0);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return obj;
	}

	Vehicle SpawnVehicle(const Hash& model, const bool& warp_into)
	{
		Vehicle veh = 0;

		TRY
		{
			if (!RequestModel(model))
			{
				std::cout << __FUNCTION__ << ": Couldn't spawn vehicle " << LOG_HEX(model) << "!\n";
				return 0;
			}

			Vector3 coords = ENTITY::GET_ENTITY_COORDS(g_LocalPlayer.m_Entity, TRUE, TRUE);
			veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z,
				ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity), FALSE, FALSE, FALSE, FALSE);

			if (!veh)
			{
				std::cout << __FUNCTION__ << ": Couldn't spawn vehicle " << LOG_HEX(model) << "!\n";
				return veh;
			}

			YieldThread();

			DECORATOR::DECOR_SET_BOOL(veh, "wagon_block_honor", TRUE);
			VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh, 0);
			if (warp_into)
				PED::SET_PED_INTO_VEHICLE(g_LocalPlayer.m_Entity, veh, -1);

			ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
			STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
		}
		EXCEPT{ LOG_EXCEPTION(); }
		
		return veh;
	}
}
