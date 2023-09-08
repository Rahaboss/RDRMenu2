#include "pch.h"
#include "Entity.h"
#include "Rage/natives.h"
#include "Rage/fwBasePool.h"

void Script::SetEntityCoords(Entity ent, const Vector3& pos)
{
	ENTITY::SET_ENTITY_COORDS(ent, pos.x, pos.y, pos.z, false, false, false, false);
}

void Script::PlaceEntityOnGround(Entity ent)
{
	ENTITY::PLACE_ENTITY_ON_GROUND_PROPERLY(ent, true);
}

void Script::SetEntityInvincible(Ped ped, bool Toggle)
{
	ENTITY::SET_ENTITY_INVINCIBLE(ped, Toggle);
}

std::vector<Ped> Script::GetAllPeds()
{
	std::vector<Ped> result{};
	const auto pool = rage::fwBasePool::GetPedPool();

	if (!pool)
		return result;

	for (uint32_t i = 0; i < pool->m_Size; i++)
	{
		if (pool->IsValid(i))
		{
			int64_t* obj = pool->GetAt<int64_t>(i);
			if (obj)
			{
				uint32_t ent = Pointers::fwScriptGuidCreateGuid(obj);
				if (DoesEntityExist(ent))
					result.push_back(ent);
			}
		}
	}

	return result;
}

std::vector<Object> Script::GetAllObjects()
{
	std::vector<Object> result{};
	const auto pool = rage::fwBasePool::GetObjectPool();

	if (!pool)
		return result;

	for (uint32_t i = 0; i < pool->m_Size; i++)
	{
		if (pool->IsValid(i))
		{
			int64_t* obj = pool->GetAt<int64_t>(i);
			if (obj)
			{
				uint32_t ent = Pointers::fwScriptGuidCreateGuid(obj);
				if (DoesEntityExist(ent))
					result.push_back(ent);
			}
		}
	}

	return result;
}

std::vector<Vehicle> Script::GetAllVehicles()
{
	std::vector<Vehicle> result{};
	const auto pool = rage::fwBasePool::GetVehiclePool();

	if (!pool)
		return result;

	for (uint32_t i = 0; i < pool->m_Size; i++)
	{
		if (pool->IsValid(i))
		{
			int64_t* obj = pool->GetAt<int64_t>(i);
			if (obj)
			{
				uint32_t ent = Pointers::fwScriptGuidCreateGuid(obj);
				if (DoesEntityExist(ent))
					result.push_back(ent);
			}
		}
	}

	return result;
}

std::vector<Pickup> Script::GetAllPickups()
{
	std::vector<Pickup> result{};
	const auto pool = rage::fwBasePool::GetPickupPool();

	if (!pool)
		return result;

	for (uint32_t i = 0; i < pool->m_Size; i++)
	{
		if (pool->IsValid(i))
		{
			int64_t* obj = pool->GetAt<int64_t>(i);
			if (obj)
			{
				uint32_t ent = Pointers::fwScriptGuidCreateGuid(obj);
				if (DoesEntityExist(ent))
					result.push_back(ent);
			}
		}
	}

	return result;
}

bool Script::DoesEntityExist(Entity ent)
{
	TRY
	{
		return Pointers::DoesEntityExist(ent);
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return false;
}

Hash Script::GetEntityModel(Entity ent)
{
	TRY
	{
		return Pointers::GetEntityModel(ent);
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return 0;
}

Vector3 Script::GetEntityCoords(Entity ent, bool RealCoords)
{
	Vector3 Result{};
	TRY
	{
		Pointers::GetEntityCoords(Result, ent, true, RealCoords);
	}
	EXCEPT{ LOG_EXCEPTION(); }
	return Result;
}
