#pragma once

#include "pch.h"
#include "Rage/natives.h"

namespace Features
{
	void AddMoney(int AmountCents);
	void ClearWanted();
	void FixAttributes(Ped ped);
	void FixHorseAttributes(Ped ped);
	void GetLocalPlayerInfo();
	Entity GetMainEntity();
	int GetMoney();
	Entity GetMountOrVehicle();
	bool LoadGround(float x, float y, float z);
	bool LoadGround(Vector3 Location);
	void NoSliding();
	void RemoveMoney(int AmountCents);
	void ResetPlayerModel();
	void RestoreHorseCores();
	void RestorePlayerCores();
	void SetGodmode(bool Toggle);
	void SetMaxAttributeValue(Ped ped, int attr);
	void SetMoney(int AmountCents);
	void SetPlayerModel(Hash Model = RAGE_JOAAT("U_F_M_RHDNUDEWOMAN_01"));
	void SetNoRagdoll(bool Toggle);
	void SpawnBadHonorEnemy(Hash Model = RAGE_JOAAT("U_F_M_RHDNUDEWOMAN_01"));
	void SpawnGoodHonorEnemy(Hash Model = RAGE_JOAAT("U_F_M_RHDNUDEWOMAN_01"));
	void SuperRun();
	void Teleport(float x, float y, float z);
	void Teleport(const Vector3& pos);
	void TeleportOnGround(float x, float y, float z);
	void TeleportOnGround(const Vector3& pos);
	void TeleportThroughDoor();
	void TeleportToWaypoint();

	bool IsArthurModel();
	bool IsJohnModel();
}
