#pragma once

#include "pch.h"
#include "rage/natives.h"

namespace Features
{
	void DeletePed(Ped Handle);
	void EndSpawnPed(Ped Handle);
	void EndSpawnVehicle(Hash model, Vehicle Handle);
	std::string GetPedModelName(Hash hash);
	std::string GetVehicleModelName(Hash hash);
	bool RequestModel(Hash model);
	Ped SpawnPed(Hash model);
	Pickup SpawnPickup(Hash PickupHash);
	Vehicle SpawnVehicle(Hash model, bool warp_into = true);
}
