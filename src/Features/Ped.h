#pragma once

#include "pch.h"
#include "rage/natives.h"

namespace Features
{
	void CleanPed(Ped ped);
	void PrintPedAttributes(Ped ped);
	void GiveGoldCores(Ped ped);
	void SetMetapedOutfit(Ped ped, Hash outfit);
	void RemoveAllPedWeapons(Ped ped);
}
