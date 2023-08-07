#pragma once

#include "pch.h"
#include "Rage/Natives.h"

namespace Features
{
	void CleanPed(Ped ped);
	void PrintPedAttributes(Ped ped);
	void GiveGoldCores(Ped ped);
	void SetMetaPedOutfit(Ped ped, Hash outfit);
	void SetPedOutfitPreset(Ped ped, int preset, bool keep_acc = false);
	void RemoveAllPedWeapons(Ped ped);
	void StartPedScenario(Ped ped, Hash scenario, int duration = -1, Hash conditional = 0);
	void StopPedScenario(Ped ped);
	void SetPedScale(Ped ped, float scale = 1.0f);
}
