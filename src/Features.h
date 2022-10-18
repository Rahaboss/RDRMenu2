#pragma once

#include "pch.h"
#include "rage/joaat.h"
#include "rage/natives.h"
#include "rage/enums.h"

#define TOGGLE_AND_LOG_BOOL(v) v = !v;\
std::cout << #v": " << (v ? "enabled" : "disabled") << '\n';

namespace Features
{
	// To be used with TOGGLE_AND_LOG_BOOL
	inline bool EnableNoSnipers = true, EnableNoReload = true, EnableNoSliding = true,
		EnablePedSpawnLogging = false, EnableHumanSpawnLogging = false, EnableVehicleSpawnLogging = false,
		EnableAddInventoryItemLogging = true;

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
	void GiveGoldCores(const Ped& ped);
	void GiveWeapon(const Hash& weapon_hash);
	void GiveAllWeapons();
	void GiveAmmo(const Hash& ammo_hash);
	void GiveAllAmmo();
	void GiveAllDualWieldWeapons();
	void DropCurrentWeapon();
	void ClearWanted();
	void RevealMap();
	void PrintNativeHandlerAddress(const uint64_t& hash);
	void AddMoney(const int& amount_cents);
	void RemoveMoney(const int& amount_cents);
	void SetMoney(const int& amount_cents);
	bool RequestModel(const Hash& model);
	Ped SpawnPed(const Hash& model);
	Vehicle SpawnVehicle(const Hash& model, const bool& warp_into = true);
	void SetSnowType(eSnowCoverageType type);
	void NoSliding();
	//void SpawnLegendaryAnimal(const Hash& model_hash, const Hash& legendary_hash, const Hash& outfit_hash);
	void UnlockSPPreorderBonus();
	void RestorePlayerCores();
	void RestoreHorseCores();
	std::string_view GetPedModelName(const Hash& hash);
	std::string_view GetVehicleModelName(const Hash& hash);
	void GiveLeftHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveRightHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveBackWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveShoulderWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	CPedFactory* GetPedFactory();
	void* GetBlipCollection();
	void GiveCivilWarHat();
	void TeleportThroughDoor();
	bool IsKeyHeld(DWORD vKey);
	bool IsKeyClicked(DWORD vKey);
	void GiveAgedPirateRum(); // Stamina XP
	void GiveGinsengElixir(); // Health XP
	void GiveValerianRoot(); // Dead Eye XP
	// Seems to work on consumables but not clothing
	void GiveSingleInventoryItem(Hash ItemHash, Hash ItemSlot = 1084182731, int InventoryID = 1, Hash AddReason = ADD_REASON_DEFAULT);
	void GiveInventoryItem(Hash ItemHash, int Amount);
	Hash GetInventoryItemType(Hash ItemHash);
	Hash GetInventorySlot(Hash ItemHash);
}
