#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void SetEntityCoords(Entity ent, const Vector3& pos);
	Vector3 GetEntityCoords(Entity ent, bool RealCoords = true);
	void PlaceEntityOnGround(Entity ent);
	void SetEntityInvincible(Ped ped, bool Toggle);
	std::vector<Ped> GetAllPeds();
	std::vector<Object> GetAllObjects();
	std::vector<Vehicle> GetAllVehicles();
	std::vector<Pickup> GetAllPickups();
	bool DoesEntityExist(Entity ent);
	Hash GetEntityModel(Entity ent);
	int GetEntityHealth(Entity ent);
	void SetEntityInvisible(Entity ent, bool Toggle);
	void SetEntityProofs(Entity ent, uint8_t ProofBitset, bool SpecialFlag);
	void SetEntityProofs(Entity ent, bool Bullet, bool Fire, bool Explosion, bool Collision, bool Melee,
		bool Steam, bool Smoke, bool Headshot, bool SpecialFlag);
}
