#pragma once

#include "pch.h"
#include "rage/natives.h"

namespace Features
{
	void DeletePed(Ped Handle);
	void EndSpawnPed(Hash Model, Ped Handle);
	void EndSpawnVehicle(Hash model, Vehicle Handle);
	std::string GetPedModelName(const Hash& hash);
	std::string GetVehicleModelName(const Hash& hash);
	bool RequestModel(const Hash& model);
	//void SpawnLegendaryAnimal(const Hash& model_hash, const Hash& legendary_hash, const Hash& outfit_hash);
	Ped SpawnPed(const Hash& model);
	Pickup SpawnPickup(Hash PickupHash);
	Vehicle SpawnVehicle(const Hash& model, const bool& warp_into = true);
}
