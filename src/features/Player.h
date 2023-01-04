#pragma once

#include "pch.h"
#include "rage/natives.h"
#include "rage/enums.h"

namespace Features
{
	void AddMoney(const int& amount_cents);
	void ClearWanted();
	void FixAttributes(Ped ped);
	void FixHorseAttributes(Ped ped);
	void GetLocalPlayerInfo();
	Entity GetMainEntity();
	int GetMoney();
	Entity GetMountOrVehicle();
	void GiveGoldCores(const Ped& ped);
	void NoSliding();
	void PrintPedAttributes(Ped ped);
	void RemoveMoney(const int& amount_cents);
	void RestoreHorseCores();
	void RestorePlayerCores();
	void SetGodmode(bool Toggle);
	void SetMaxAttributeValue(Ped ped, int attr);
	void SetMoney(const int& amount_cents);
	void SpawnBadHonorEnemy(Hash Model = U_F_M_RHDNUDEWOMAN_01);
	void SpawnGoodHonorEnemy(Hash Model = U_F_M_RHDNUDEWOMAN_01);
	void Teleport(float x, float y, float z);
	void Teleport(const Vector3& pos);
	void TeleportOnGround(float x, float y, float z);
	void TeleportOnGround(const Vector3& pos);
	void TeleportThroughDoor();
	void TeleportToWaypoint();
}
