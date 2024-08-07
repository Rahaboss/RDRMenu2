#include "pch.h"
#include "Spawning.h"
#include "Rage/natives.h"
#include "Thread/Thread.h"
#include "PlayerInfo.h"
#include "Entity.h"
#include "Rage/enums.h"
#include "Config/Lists.h"
#include "Notification.h"
#include "Util/String.h"
#include "Memory/Pointers.h"

bool Script::IsModelValid(Hash Model)
{
	return STREAMING::IS_MODEL_IN_CDIMAGE(Model) && STREAMING::IS_MODEL_VALID(Model);
}

bool Script::RequestModel(Hash Model)
{
	if (!IsModelValid(Model))
	{
		const std::string& Name = Lists::GetHashName(Model);
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": %s is not a valid model hash!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": 0x%X is not a valid model hash!", Model);

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
	const std::string& Name = Lists::GetHashName(Model);
	if (!RequestModel(Model))
	{
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": Couldn't request ped %s!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": Couldn't request ped 0x%X!", Model);

		return 0;
	}

	const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

	const Ped ped = PED::CREATE_PED(Model, coords.x, coords.y, coords.z, g_LocalPlayer.m_Heading, false, false, false, false);

	if (!ped)
	{
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": Couldn't spawn ped %s!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": Couldn't spawn ped 0x%X!", Model);
		ReleaseModel(Model);

		return 0;
	}

	Thread::YieldThread();

	PED::_SET_RANDOM_OUTFIT_VARIATION(ped, true);
	PlaceEntityOnGround(ped);
	ReleaseModel(Model);

	NotifyFeed(Util::FormatString("Spawned %s", Name.c_str()).c_str());

	return ped;
}

Vehicle Script::SpawnVehicle(Hash Model, bool WarpInside)
{
	const std::string& Name = Lists::GetHashName(Model);
	if (!RequestModel(Model))
	{
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": Couldn't request vehicle %s!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": Couldn't request vehicle 0x%X!", Model);
		
		return 0;
	}

	const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

	const Vehicle veh = VEHICLE::CREATE_VEHICLE(Model, coords.x, coords.y, coords.z, g_LocalPlayer.m_Heading, false, false, false, false);

	if (!veh)
	{
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": Couldn't spawn vehicle %s!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": Couldn't spawn vehicle 0x%X!", Model);
		ReleaseModel(Model);

		return 0;
	}

	Thread::YieldThread();

	DECORATOR::DECOR_SET_BOOL(veh, "wagon_block_honor", true);
	PlaceEntityOnGround(veh);
	ReleaseModel(Model);

	if (WarpInside)
		PED::SET_PED_INTO_VEHICLE(g_LocalPlayer.m_Entity, veh, -1);

	NotifyFeed(Util::FormatString("Spawned %s", Name.c_str()).c_str());

	return veh;
}

Object Script::SpawnObject(Hash Model)
{
	const std::string& Name = Lists::GetHashName(Model);
	if (!RequestModel(Model))
	{
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": Couldn't request object %s!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": Couldn't request object 0x%X!", Model);

		return 0;
	}

	const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

	const Object obj = OBJECT::CREATE_OBJECT_NO_OFFSET(Model, coords.x, coords.y, coords.z, true, true, false, true);

	if (!obj)
	{
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": Couldn't spawn object %s!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": Couldn't spawn object 0x%X!", Model);
		ReleaseModel(Model);

		return 0;
	}

	Thread::YieldThread();

	PlaceEntityOnGround(obj);
	ReleaseModel(Model);

	NotifyFeed(Util::FormatString("Spawned %s", Name.c_str()).c_str());

	return obj;
}

Pickup Script::SpawnPickup(Hash PickupHash, Hash Model)
{
	const std::string& Name = Lists::GetHashName(Model);
	if (Model && !RequestModel(Model))
	{
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": Couldn't request pickup %s!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": Couldn't request pickup 0x%X!", Model);
		
		return 0;
	}

	const Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(g_LocalPlayer.m_Entity, 0.0, 3.0, -0.3);

	const Pickup pickup = OBJECT::CREATE_PICKUP(PickupHash, coords.x, coords.y, coords.z, 0, 0, false, Model, 0, 0.0f, 0);

	if (!pickup)
	{
		if (!Name.empty())
			LOG_TEXT(__FUNCTION__": Couldn't spawn pickup %s!", Name.c_str());
		else
			LOG_TEXT(__FUNCTION__": Couldn't spawn pickup 0x%X!", PickupHash);
		ReleaseModel(Model);

		return 0;
	}
	
	Thread::YieldThread();

	PlaceEntityOnGround(OBJECT::GET_PICKUP_OBJECT(pickup));
	if (Model)
		ReleaseModel(Model);

	NotifyFeed(Util::FormatString("Spawned %s", Name.c_str()).c_str());

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

void Script::SpawnGoodHonorEnemy()
{
	if (Ped ped = SpawnPed(RAGE_JOAAT("CS_FAMOUSGUNSLINGER_04")))
	{
		DECORATOR::DECOR_SET_INT(ped, "HONOR_OVERRIDE", -9999);
		TASK::TASK_COMBAT_PED(ped, g_LocalPlayer.m_Entity, 0, 0);
	}
}

void Script::SpawnBadHonorEnemy()
{
	if (Ped ped = SpawnPed(RAGE_JOAAT("CS_FAMOUSGUNSLINGER_04")))
	{
		DECORATOR::DECOR_SET_INT(ped, "HONOR_OVERRIDE", 9999);
		TASK::TASK_COMBAT_PED(ped, g_LocalPlayer.m_Entity, 0, 0);
	}
}

const char* Script::GetStringFromHashKey(Hash Label)
{
	TRY
	{
		return Pointers::GetStringFromHashKey(Label);
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return nullptr;
}
