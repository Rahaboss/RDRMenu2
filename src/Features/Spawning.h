#pragma once

#include "pch.h"
#include "rage/natives.h"

namespace Features
{
	void DeleteObject(Ped Handle);
	void DeletePed(Ped Handle);
	void EndSpawnPed(Ped Handle);
	void EndSpawnVehicle(Hash model, Vehicle Handle);
	std::string GetPedModelName(Hash hash);
	std::string GetVehicleModelName(Hash hash);
	bool IsModelValid(Hash Model);
	bool RequestModel(Hash model);
	Object SpawnObject(Hash model);
	Ped SpawnPed(Hash model);
	Pickup SpawnPickup(Hash PickupHash);
	Vehicle SpawnVehicle(Hash model, bool warp_into = true);
}
