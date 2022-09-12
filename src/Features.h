#pragma once

#include "pch.h"
#include "rage/joaat.h"
#include "rage/natives.h"

namespace Features
{
	inline bool EnableNoSnipers = true;

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
	void GiveWeapon(const Hash& weapon_hash);
	void GiveAllWeapons();
	void GiveGoldCores(const Ped& ped);
	void GiveAllAmmo();
	void ClearWanted();
	void RevealMap();
	void PrintNativeHandlerAddress(const uint64_t& hash);
	void AddMoney(const int& amount_cents);
	void RemoveMoney(const int& amount_cents);
	void SetMoney(const int& amount_cents);
	bool RequestModel(const Hash& model);
	void SpawnPed(const Hash& model);
	void SpawnVehicle(const Hash& model, const bool& warp_into = true);
	void SetSnowType(eSnowCoverageType type);
	void NoSliding();
}
