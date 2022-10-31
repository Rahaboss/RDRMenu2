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
	inline bool
		EnableNoSnipers = true,
		EnableNoReload = true,
		EnableNoSliding = true,
		EnablePedSpawnLogging = false,
		EnableHumanSpawnLogging = false,
		EnableVehicleSpawnLogging = false,
		EnableAddInventoryItemLogging = false,
		EnableGodMode = true;
	inline uint8_t g_rgb[3]{ 255, 0, 0 };


	/// DEBUG
	void* GetBlipCollection();
	CPedFactory* GetPedFactory();
	void PrintNativeHandlerAddress(const uint64_t& hash);
	void UnlockSPPreorderBonus();


	/// INPUT
	bool IsKeyClicked(DWORD vKey);
	bool IsKeyHeld(DWORD vKey);


	/// INVENTORY
	Hash GetInventoryItemType(Hash ItemHash);
	Hash GetInventorySlot(Hash ItemHash);
	void GiveAgedPirateRum(); // Stamina XP
	void GiveCivilWarHat();
	void GiveGinsengElixir(); // Health XP
	void GiveInventoryItem(Hash ItemHash, int Amount);
	void GiveSingleInventoryItem(Hash ItemHash, Hash ItemSlot = 1084182731, int InventoryID = 1, Hash AddReason = ADD_REASON_DEFAULT);
	void GiveValerianRoot(); // Dead Eye XP


	/// PLAYER
	void AddMoney(const int& amount_cents);
	void ClearWanted();
	void GetLocalPlayerInfo();
	Entity GetMainEntity();
	Entity GetMountOrVehicle();
	void GiveGoldCores(const Ped& ped);
	void NoSliding();
	void SetGodmode(bool Toggle);
	void SetMoney(const int& amount_cents);
	void RemoveMoney(const int& amount_cents);
	void RestoreHorseCores();
	void RestorePlayerCores();
	void SpawnBadHonorEnemy(Hash Model = U_F_M_RHDNUDEWOMAN_01);
	void SpawnGoodHonorEnemy(Hash Model = U_F_M_RHDNUDEWOMAN_01);
	void Teleport(const float& x, const float& y, const float& z);
	void Teleport(const Vector3& pos);
	void TeleportThroughDoor();
	void TeleportToWaypoint();


	/// RENDERING
	void RenderText(const char* Text, float PosX, float Posy, uint8_t ColR = 255, uint8_t ColG = 255, uint8_t ColB = 255, uint8_t ColA = 255, float Scale = 1.0f);
	void RenderTextOnEntity(Entity Ent, const char* Text);
	bool WorldToScreen(float X, float Y, float Z, float& OutX, float& OutY);


	/// SPAWNING
	std::string_view GetPedModelName(const Hash& hash);
	std::string_view GetVehicleModelName(const Hash& hash);
	bool RequestModel(const Hash& model);
	//void SpawnLegendaryAnimal(const Hash& model_hash, const Hash& legendary_hash, const Hash& outfit_hash);
	Ped SpawnPed(const Hash& model);
	Pickup SpawnPickup(Hash PickupHash);
	Vehicle SpawnVehicle(const Hash& model, const bool& warp_into = true);


	/// WEAPONS
	void DropCurrentWeapon();
	void GiveAmmo(const Hash& ammo_hash);
	void GiveAllAmmo();
	void GiveAllDualWieldWeapons();
	void GiveAllWeapons();
	void GiveBackWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveLeftHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveRightHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveShoulderWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveWeapon(const Hash& weapon_hash);


	/// WEAPONS
	void RevealMap();
	void SetSnowType(eSnowCoverageType type);


	/// MISC
	void ExecuteAsThread(joaat_t script_hash, void (*function)());
	void Setup();
	void OnTick();
	void RunJobQueue();
	int FpsTick();
	void YieldThread();
	void RGBTick();
}
