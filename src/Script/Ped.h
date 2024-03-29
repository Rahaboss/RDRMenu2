#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/enums.h"

namespace Script
{
	void GiveGoldCores(Ped ped);
	void FillCores(Ped ped);
	bool DoesPedBoneExist(Ped ped, int boneId);
	Vector3 GetPedBoneCoords(Ped ped, int boneId);
	void SetPedOntoMount(Ped ped, Ped Mount, int Seat = -1);
	void SetPedNoRagdoll(Ped ped, bool Toggle);
	void SetPedOutfitPreset(Ped ped, int Preset, bool KeepAccessories = false);
	void SetMetaPedOutfit(Ped ped, Hash Outfit);
	void CleanPed(Ped ped);
	void NoSliding(Ped ped);
	void SuperRun(Ped ped);
	eMetaPedType GetPedType(Ped ped);
	void StartPedScenario(Ped ped, Hash Scenario, int Duration = -1, Hash Conditional = 0);
	void StopPedScenario(Ped ped);
	void StopPedScenarioImmediatelyAndClearProps(Ped ped);
}
