#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	Hash GetCurrentWeapon();
	void RapidFire();
	void GiveAmmo(Ped ped, Hash AmmoHash);
	void GiveWeapon(Ped ped, Hash WeaponHash);
	void RGBElectricLantern();
	void GiveAllWeapons();
	void GiveAllAmmo();
}
