#pragma once

#include "pch.h"
#include "rage/natives.h"

namespace Features
{
	void DeleteObject(Ped Handle);
	void DeletePed(Ped Handle);
	void DeleteVehicle(Vehicle Handle);
	std::string GetPedModelName(Hash hash);
	std::string GetVehicleModelName(Hash hash);
	bool IsModelValid(Hash Model);
	bool RequestModel(Hash Model);
	Object SpawnObject(Hash Model);
	Ped SpawnPed(Hash Model);
	Pickup SpawnPickup(Hash PickupHash);
	Vehicle SpawnVehicle(Hash Model, bool WarpInside = true);
	void LogSpawnedEntity(Entity ent);
}
