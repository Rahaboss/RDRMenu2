#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void SetEntityCoords(Entity ent, const Vector3& pos);
	void PlaceEntityOnGround(Entity ent);
	void SetInvincible(Ped ped, bool Toggle);
	std::vector<Ped> GetAllPeds();
	std::vector<Object> GetAllObjects();
	std::vector<Vehicle> GetAllVehicles();
	std::vector<Pickup> GetAllPickups();
	bool DoesEntityExist(Entity ent);
	Hash GetEntityModel(Entity ent);
	Vector3 GetEntityCoords(Entity ent, bool RealCoords = true);
}