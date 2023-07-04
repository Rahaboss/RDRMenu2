#pragma once

#include "pch.h"
#include "Rage/natives.h"
#include "Rage/enums.h"

namespace Features
{
	void DropCurrentWeapon();
	Hash GetCurrentWeapon();
	void GiveAmmo(Hash AmmoHash);
	void GiveAllAmmo();
	void GiveAllDualWieldWeapons();
	void GiveAllWeapons();
	void GiveBackWeapon(Hash WeaponHash, int AmmoAmount = 9999);
	void GiveLeftHandWeapon(Hash WeaponHash, int AmmoAmount = 9999);
	void GiveRightHandWeapon(Hash WeaponHash, int AmmoAmount = 9999);
	void GiveShoulderWeapon(Hash WeaponHash, int AmmoAmount = 9999);
	void GiveWeapon(Hash WeaponHash);
	void RemoveWeapon(Hash WeaponHash);
	void RGBElectricLantern();
	void RapidFire();
}
