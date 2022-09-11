#pragma once

#include "pch.h"
#include "rage/joaat.h"
#include "rage/natives.h"

namespace Features
{
	void ExecuteAsThread(joaat_t script_hash, void (*function)());
	void Setup();
	void OnTick();
	void RunJobQueue();
	int FpsTick();
	void YieldThread();
	void GetLocalPlayerInfo();
	Entity GetMountOrVehicle();
	Entity GetMainEntity();
	void Teleport(const float& x, const float& y, const float& z);
	void Teleport(const Vector3& pos);
	void TeleportToWaypoint();
}
