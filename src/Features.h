#pragma once

#include "pch.h"
#include "rage/joaat.h"
#include "rage/natives.h"

namespace Features
{
	inline bool EnableNoSnipers = true, EnableNoReload = true, EnableNoSliding = true;

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
	//void SpawnLegendaryAnimal(const Hash& model_hash, const Hash& legendary_hash, const Hash& outfit_hash);
	void UnlockSPPreorderBonus();
	void RestorePlayerCores();
	std::string_view GetModelName(const Hash& hash);
	void GiveLeftHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveRightHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveBackWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveShoulderWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
}
