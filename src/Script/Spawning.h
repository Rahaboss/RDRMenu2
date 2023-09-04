#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	bool IsModelValid(Hash Model);
	bool RequestModel(Hash Model);
	void ReleaseModel(Hash Model);
	Ped SpawnPed(Hash Model);
	Vehicle SpawnVehicle(Hash Model, bool WarpInside = true);
	Object SpawnObject(Hash Model);
	Pickup SpawnPickup(Hash PickupHash, Hash Model = 0);
}
