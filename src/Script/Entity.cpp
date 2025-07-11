#include "pch.h"
#include "Entity.h"
#include "Rage/natives.h"
#include "Rage/fwBasePool.h"

void Script::SetEntityCoords(Entity ent, const Vector3& pos)
{
	ENTITY::SET_ENTITY_COORDS(ent, pos.x, pos.y, pos.z, false, false, false, false);
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
	std::vector<Ped> result;

	if (const rage::fwBasePool* pool = rage::fwBasePool::GetPedPool())
	{
		for (uint32_t i = 0; i < pool->m_Size; i++)
		{
			if (pool->IsValid(i))
			{
				if (void* obj = pool->GetAt(i))
				{
					uint32_t ent = Pointers::fwScriptGuidCreateGuid(obj);
					if (DoesEntityExist(ent))
						result.push_back(ent);
				}
			}
		}
	}

	return result;
}

std::vector<Object> Script::GetAllObjects()
{
	std::vector<Object> result;

	if (const rage::fwBasePool* pool = rage::fwBasePool::GetObjectPool())
	{
		for (uint32_t i = 0; i < pool->m_Size; i++)
		{
			if (pool->IsValid(i))
			{
				if (void* obj = pool->GetAt(i))
				{
					uint32_t ent = Pointers::fwScriptGuidCreateGuid(obj);
					if (DoesEntityExist(ent))
						result.push_back(ent);
				}
			}
		}
	}

	return result;
}

std::vector<Vehicle> Script::GetAllVehicles()
{
	std::vector<Vehicle> result;
	
	if (const rage::fwBasePool* pool = rage::fwBasePool::GetVehiclePool())
	{
		for (uint32_t i = 0; i < pool->m_Size; i++)
		{
			if (pool->IsValid(i))
			{
				if (void* obj = pool->GetAt(i))
				{
					uint32_t ent = Pointers::fwScriptGuidCreateGuid(obj);
					if (DoesEntityExist(ent))
						result.push_back(ent);
				}
			}
		}
	}

	return result;
}

std::vector<Pickup> Script::GetAllPickups()
{
	std::vector<Pickup> result;

	if (const rage::fwBasePool* pool = rage::fwBasePool::GetPickupPool())
	{
		for (uint32_t i = 0; i < pool->m_Size; i++)
		{
			if (pool->IsValid(i))
			{
				if (void* obj = pool->GetAt(i))
				{
					uint32_t ent = Pointers::fwScriptGuidCreateGuid(obj);
					if (DoesEntityExist(ent))
						result.push_back(ent);
				}
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

int Script::GetEntityHealth(Entity ent)
{
	TRY
	{
		return Pointers::GetEntityHealth(ent);
	}
	EXCEPT{ LOG_EXCEPTION(); }

	return 0;
}

void Script::SetEntityInvisible(Entity ent, bool Toggle)
{
	ENTITY::SET_ENTITY_VISIBLE(ent, !Toggle);
}

void Script::SetEntityProofs(Entity ent, uint8_t ProofBitset, bool SpecialFlag)
{
	Pointers::SetEntityProofs(ent, ProofBitset, SpecialFlag);
}

void Script::SetEntityProofs(Entity ent, bool Bullet, bool Fire, bool Explosion, bool Collision, bool Melee,
	bool Steam, bool Smoke, bool Headshot, bool SpecialFlag)
{
	uint8_t ProofBitset = 0;
	if (Bullet)
		ProofBitset |= (1 << 0);
	if (Fire)
		ProofBitset |= (1 << 1);
	if (Explosion)
		ProofBitset |= (1 << 2);
	if (Collision)
		ProofBitset |= (1 << 3);
	if (Melee)
		ProofBitset |= (1 << 4);
	if (Steam)
		ProofBitset |= (1 << 5);
	if (Smoke)
		ProofBitset |= (1 << 6);
	if (Headshot)
		ProofBitset |= (1 << 7);
	SetEntityProofs(ent, ProofBitset, SpecialFlag);
}
