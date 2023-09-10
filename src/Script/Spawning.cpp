#include "pch.h"
#include "Spawning.h"
#include "Rage/natives.h"
#include "Thread/Thread.h"
#include "PlayerInfo.h"
#include "Entity.h"
#include "Rage/enums.h"

bool Script::IsModelValid(Hash Model)
{
	return STREAMING::IS_MODEL_IN_CDIMAGE(Model) && STREAMING::IS_MODEL_VALID(Model);
}

bool Script::RequestModel(Hash Model)
{
	if (!IsModelValid(Model))
	{
		LOG_TEXT("%s: 0x%X is not a valid model hash!", __FUNCTION__, Model);
		return false;
	}

	for (int i = 0; i < 100; i++)
	{
		STREAMING::REQUEST_MODEL(Model, false);
		Thread::YieldThread();

		if (STREAMING::HAS_MODEL_LOADED(Model))
			return true;
	}

	return false;
}

void Script::ReleaseModel(Hash Model)
{
	STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(Model);
}

Ped Script::SpawnPed(Hash Model)
{
	if (!RequestModel(Model))
	{
		LOG_TEXT("%s: Couldn't request ped 0x%X!", __FUNCTION__, Model);
		return 0;
	}

	const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

	const Ped ped = PED::CREATE_PED(Model, coords.x, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity),
		false, false, false, false);

	if (!ped)
	{
		LOG_TEXT("%s: Couldn't spawn ped 0x%X!", __FUNCTION__, Model);
		ReleaseModel(Model);
		return 0;
	}

	Thread::YieldThread();

	PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true);
	Thread::YieldThread();

	PlaceEntityOnGround(ped);
	ReleaseModel(Model);

	return ped;
}

Vehicle Script::SpawnVehicle(Hash Model, bool WarpInside)
{
	if (!RequestModel(Model))
	{
		LOG_TEXT("%s: Couldn't request vehicle 0x%X!", __FUNCTION__, Model);
		return 0;
	}

	const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

	const Vehicle veh = VEHICLE::CREATE_VEHICLE(Model, coords.x, coords.y, coords.z, ENTITY::GET_ENTITY_HEADING(g_LocalPlayer.m_Entity),
		false, false, false, false);

	if (!veh)
	{
		LOG_TEXT("%s: Couldn't spawn vehicle 0x%X!", __FUNCTION__, Model);
		ReleaseModel(Model);
		return 0;
	}

	Thread::YieldThread();

	DECORATOR::DECOR_SET_BOOL(veh, "wagon_block_honor", true);
	PlaceEntityOnGround(veh);
	ReleaseModel(Model);

	if (WarpInside)
		PED::SET_PED_INTO_VEHICLE(g_LocalPlayer.m_Entity, veh, -1);

	return veh;
}

Object Script::SpawnObject(Hash Model)
{
	if (!RequestModel(Model))
	{
		LOG_TEXT("%s: Couldn't request object 0x%X!", __FUNCTION__, Model);
		return 0;
	}

	const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

	const Object obj = OBJECT::CREATE_OBJECT_NO_OFFSET(Model, coords.x, coords.y, coords.z, true, true, false, true);

	if (!obj)
	{
		LOG_TEXT("%s: Couldn't spawn object 0x%X!", __FUNCTION__, Model);
		ReleaseModel(Model);
		return 0;
	}

	Thread::YieldThread();

	PlaceEntityOnGround(obj);
	ReleaseModel(Model);

	return obj;
}

Pickup Script::SpawnPickup(Hash PickupHash, Hash Model)
{
	if (Model && !RequestModel(Model))
	{
		LOG_TEXT("%s: Couldn't request pickup 0x%X!", __FUNCTION__, Model);
		return 0;
	}

	const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

	const Pickup pickup = OBJECT::CREATE_PICKUP(PickupHash, coords.x, coords.y, coords.z, 0, 0, false, Model, 0, 0.0f, 0);

	if (!pickup)
	{
		LOG_TEXT("%s: Couldn't spawn pickup 0x%X!", __FUNCTION__, PickupHash);
		ReleaseModel(Model);
		return 0;
	}
	
	Thread::YieldThread();

	PlaceEntityOnGround(OBJECT::GET_PICKUP_OBJECT(pickup));
	if (Model)
		ReleaseModel(Model);

	return pickup;
}

void Script::SetEntityAsNoLongerNeeded(Entity ent)
{
	ENTITY::SET_ENTITY_AS_NO_LONGER_NEEDED(&ent);
}

void Script::DeleteEntity(Entity Handle)
{
	ENTITY::SET_ENTITY_AS_MISSION_ENTITY(Handle, true, true);

	switch (ENTITY::GET_ENTITY_TYPE(Handle))
	{
	case ET_PED:
		PED::DELETE_PED(&Handle);
		break;
	case ET_VEHICLE:
		VEHICLE::DELETE_VEHICLE(&Handle);
		break;
	case ET_OBJECT:
		OBJECT::DELETE_OBJECT(&Handle);
		break;
	}
}
