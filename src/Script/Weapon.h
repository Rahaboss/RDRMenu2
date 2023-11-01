#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/enums.h"

namespace Script
{
	Hash GetCurrentWeapon();
	void RapidFire();
	void GiveAmmo(Ped ped, Hash AmmoHash);
	void GiveWeapon(Ped ped, Hash WeaponHash, Hash AttachPoint = WEAPON_ATTACH_POINT_HAND_PRIMARY);
	void RGBElectricLantern();
	void GiveAllWeapons();
	void GiveAllAmmo();
	void DropCurrentWeapon();
}
