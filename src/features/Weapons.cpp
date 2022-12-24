#include "pch.h"
#include "Weapons.h"
#include "Features.h"
#include "PlayerInfo.h"
#include "Lists.h"
#include "JobQueue.h"

// Features for managing player weapons

namespace Features
{
	void DropCurrentWeapon()
	{
		QUEUE_JOB(=)
		{
			Hash cur = GetCurrentWeapon();
			if (WEAPON::IS_WEAPON_VALID(cur) && cur != WEAPON_UNARMED)
				WEAPON::SET_PED_DROPS_INVENTORY_WEAPON(g_LocalPlayer.m_Entity, cur, 0.0, 0.0, 0.0, 1);
		}
		END_JOB()
	}

	Hash GetCurrentWeapon()
	{
		Hash CurWeap = 0;
		WEAPON::GET_CURRENT_PED_WEAPON(g_LocalPlayer.m_Entity, &CurWeap, FALSE, WEAPON_ATTACH_POINT_HAND_PRIMARY, FALSE);
		return CurWeap;
	}

	void GiveAmmo(const Hash& ammo_hash)
	{
		QUEUE_JOB(=)
		{
			WEAPON::_ADD_AMMO_TO_PED_BY_TYPE(g_LocalPlayer.m_Entity, ammo_hash, 9999, ADD_REASON_DEFAULT);
		}
		END_JOB()
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
			GiveWeapon(w.second);
	}

	void GiveBackWeapon(const Hash& WeaponHash, const int& AmmoAmount)
	{
		QUEUE_JOB(=)
		{
			if (!WEAPON::_IS_WEAPON_TWO_HANDED(WeaponHash))
			{
				std::cout << __FUNCTION__ << ": Weapon " << LOG_HEX(WeaponHash) << " (" << HUD::GET_STRING_FROM_HASH_KEY(WeaponHash) << ") is not two handed!\n";
				return;
			}

			WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, TRUE, FALSE,
				WEAPON_ATTACH_POINT_RIFLE, TRUE, 0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
		}
		END_JOB()
	}

	void GiveLeftHandWeapon(const Hash& WeaponHash, const int& AmmoAmount)
	{
		QUEUE_JOB(=)
		{
			if (!WEAPON::_IS_WEAPON_ONE_HANDED(WeaponHash))
			{
				std::cout << __FUNCTION__ << ": Weapon " << LOG_HEX(WeaponHash) << " (" << HUD::GET_STRING_FROM_HASH_KEY(WeaponHash) << ") is not one handed!\n";
				return;
			}

			WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, TRUE, FALSE,
				WEAPON_ATTACH_POINT_HAND_PRIMARY, TRUE, 0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
		}
		END_JOB()
	}

	void GiveRightHandWeapon(const Hash& WeaponHash, const int& AmmoAmount)
	{
		QUEUE_JOB(=)
		{
			if (!WEAPON::_IS_WEAPON_ONE_HANDED(WeaponHash))
			{
				std::cout << __FUNCTION__ << ": Weapon " << LOG_HEX(WeaponHash) << " (" << HUD::GET_STRING_FROM_HASH_KEY(WeaponHash) << ") is not one handed!\n";
				return;
			}

			WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, TRUE, FALSE,
				WEAPON_ATTACH_POINT_HAND_SECONDARY, TRUE, 0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
		}
		END_JOB()
	}

	void GiveShoulderWeapon(const Hash& WeaponHash, const int& AmmoAmount)
	{
		QUEUE_JOB(=)
		{
			if (!WEAPON::_IS_WEAPON_TWO_HANDED(WeaponHash))
			{
				std::cout << __FUNCTION__ << ": Weapon " << LOG_HEX(WeaponHash) << " (" << HUD::GET_STRING_FROM_HASH_KEY(WeaponHash) << ") is not two handed!\n";
				return;
			}

			WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, TRUE, FALSE,
				WEAPON_ATTACH_POINT_RIFLE_ALTERNATE, TRUE, 0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
		}
		END_JOB()
	}

	void GiveWeapon(const Hash& weapon_hash)
	{
		switch (weapon_hash)
		{
		case AMMO_MOLOTOV_VOLATILE:
		case AMMO_DYNAMITE_VOLATILE:
		case AMMO_THROWING_KNIVES_IMPROVED:
		case AMMO_THROWING_KNIVES_POISON:
		case AMMO_TOMAHAWK_IMPROVED:
		case AMMO_TOMAHAWK_HOMING:
			GiveAmmo(weapon_hash);
			break;
		default:
			QUEUE_JOB(=)
			{
				WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, weapon_hash, 9999, TRUE, FALSE, WEAPON_ATTACH_POINT_HAND_PRIMARY, TRUE,
					0.5f, 1.0f, ADD_REASON_DEFAULT, TRUE, 0.0f, FALSE);
			}
			END_JOB()
			break;
		}
	}

	void RGBElectricLantern()
	{
		if (GetCurrentWeapon() != WEAPON_MELEE_LANTERN_ELECTRIC)
			return;
		
		Entity WeapEnt = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(g_LocalPlayer.m_Entity, WEAPON_ATTACH_POINT_HAND_PRIMARY);
		
		GRAPHICS::_SET_LIGHTS_TYPE_FOR_ENTITY(WeapEnt, 1);
		GRAPHICS::_SET_LIGHTS_INTENSITY_FOR_ENTITY(WeapEnt, 5.0f);
		GRAPHICS::_SET_LIGHTS_COLOR_FOR_ENTITY(WeapEnt, g_rgb[0], g_rgb[1], g_rgb[2]);
		GRAPHICS::UPDATE_LIGHTS_ON_ENTITY(WeapEnt);

		GRAPHICS::_SET_LIGHTS_TYPE_FOR_ENTITY(g_LocalPlayer.m_Entity, 1);
		GRAPHICS::_SET_LIGHTS_INTENSITY_FOR_ENTITY(g_LocalPlayer.m_Entity, 5.0f);
		GRAPHICS::_SET_LIGHTS_COLOR_FOR_ENTITY(g_LocalPlayer.m_Entity, g_rgb[0], g_rgb[1], g_rgb[2]);
		GRAPHICS::UPDATE_LIGHTS_ON_ENTITY(g_LocalPlayer.m_Entity);
	}
}
