#pragma once

#include "pch.h"
#include "rage/natives.h"
#include "rage/enums.h"

namespace Features
{
	void DropCurrentWeapon();
	Hash GetCurrentWeapon();
	void GiveAmmo(const Hash& ammo_hash);
	void GiveAllAmmo();
	void GiveAllDualWieldWeapons();
	void GiveAllWeapons();
	void GiveBackWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveLeftHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveRightHandWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveShoulderWeapon(const Hash& WeaponHash, const int& AmmoAmount = 9999);
	void GiveWeapon(const Hash& weapon_hash);
	void RGBElectricLantern();
}
