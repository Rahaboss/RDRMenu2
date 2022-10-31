#include "pch.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "rage/lists.h"

// Features for managing player weapons

namespace Features
{
	void DropCurrentWeapon()
	{
		Hash cur;
		if (WEAPON::GET_CURRENT_PED_WEAPON(g_LocalPlayer.m_Entity, &cur, 0, 0, 0) && WEAPON::IS_WEAPON_VALID(cur) && cur != WEAPON_UNARMED)
			WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(g_LocalPlayer.m_Entity, cur, 0.0, 0.0, 0.0, 1);
	}

	void GiveAmmo(const Hash& ammo_hash)
	{
		WEAPON::_ADD_AMMO_TO_PED_BY_TYPE(g_LocalPlayer.m_Entity, ammo_hash, 9999, ADD_REASON_DEFAULT);
	}

	void GiveAllAmmo()
	{
		for (const auto& a : g_AmmoList)
			GiveAmmo(a);
	}

	void GiveAllDualWieldWeapons()
	{
		for (const auto& w : g_DualWieldWeaponPickupList)
			SpawnPickup(w);
	}

	void GiveAllWeapons()
	{
		for (const auto& w : g_WeaponList)
			GiveWeapon(w);
	}

	void GiveBackWeapon(const Hash& WeaponHash, const int& AmmoAmount)
	{
		if (!WEAPON::_IS_WEAPON_TWO_HANDED(WeaponHash))
		{
			std::cout << __FUNCTION__ << ": Weapon " << LOG_HEX(WeaponHash) << " (" << HUD::GET_STRING_FROM_HASH_KEY(WeaponHash) << ") is not two handed!\n";
			return;
		}

		WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, TRUE, FALSE,
			WEAPON_ATTACH_POINT_RIFLE, TRUE, 0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
	}

	void GiveLeftHandWeapon(const Hash& WeaponHash, const int& AmmoAmount)
	{
		if (!WEAPON::_IS_WEAPON_ONE_HANDED(WeaponHash))
		{
			std::cout << __FUNCTION__ << ": Weapon " << LOG_HEX(WeaponHash) << " (" << HUD::GET_STRING_FROM_HASH_KEY(WeaponHash) << ") is not one handed!\n";
			return;
		}

		WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, TRUE, FALSE,
			WEAPON_ATTACH_POINT_HAND_PRIMARY, TRUE, 0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
	}

	void GiveRightHandWeapon(const Hash& WeaponHash, const int& AmmoAmount)
	{
		if (!WEAPON::_IS_WEAPON_ONE_HANDED(WeaponHash))
		{
			std::cout << __FUNCTION__ << ": Weapon " << LOG_HEX(WeaponHash) << " (" << HUD::GET_STRING_FROM_HASH_KEY(WeaponHash) << ") is not one handed!\n";
			return;
		}

		WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, TRUE, FALSE,
			WEAPON_ATTACH_POINT_HAND_SECONDARY, TRUE, 0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
	}

	void GiveShoulderWeapon(const Hash& WeaponHash, const int& AmmoAmount)
	{
		if (!WEAPON::_IS_WEAPON_TWO_HANDED(WeaponHash))
		{
			std::cout << __FUNCTION__ << ": Weapon " << LOG_HEX(WeaponHash) << " (" << HUD::GET_STRING_FROM_HASH_KEY(WeaponHash) << ") is not two handed!\n";
			return;
		}

		WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, TRUE, FALSE,
			WEAPON_ATTACH_POINT_RIFLE_ALTERNATE, TRUE, 0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
	}

	void GiveWeapon(const Hash& weapon_hash)
	{
		WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, weapon_hash, 9999, TRUE, FALSE, WEAPON_ATTACH_POINT_HAND_PRIMARY, TRUE,
			0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
	}
}
