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
		WEAPON::GET_CURRENT_PED_WEAPON(g_LocalPlayer.m_Entity, &CurWeap, false, WEAPON_ATTACH_POINT_HAND_PRIMARY, false);
		return CurWeap;
	}

	void GiveAmmo(Hash AmmoHash)
	{
		QUEUE_JOB(=)
		{
			WEAPON::_ADD_AMMO_TO_PED_BY_TYPE(g_LocalPlayer.m_Entity, AmmoHash, 9999, ADD_REASON_DEFAULT);
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

	void GiveBackWeapon(Hash WeaponHash, int AmmoAmount)
	{
		QUEUE_JOB(=)
		{
			if (!WEAPON::_IS_WEAPON_TWO_HANDED(WeaponHash))
			{
				LOG_TO_CONSOLE("%s: Weapon 0x%X (%s) is not two handed!\n", __FUNCTION__, WeaponHash, HUD::GET_STRING_FROM_HASH_KEY(WeaponHash));
				return;
			}

			WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, true, false,
				WEAPON_ATTACH_POINT_RIFLE, true, 0.5f, 1.0f, ADD_REASON_DEFAULT, true, 0.0f, false);
		}
		END_JOB()
	}

	void GiveLeftHandWeapon(Hash WeaponHash, int AmmoAmount)
	{
		QUEUE_JOB(=)
		{
			if (!WEAPON::_IS_WEAPON_ONE_HANDED(WeaponHash))
			{
				LOG_TO_CONSOLE("%s: Weapon 0x%X (%s) is not two handed!\n", __FUNCTION__, WeaponHash, HUD::GET_STRING_FROM_HASH_KEY(WeaponHash));
				return;
			}

			WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, true, false,
				WEAPON_ATTACH_POINT_HAND_PRIMARY, true, 0.5f, 1.0f, ADD_REASON_DEFAULT, true, 0.0f, false);
		}
		END_JOB()
	}

	void GiveRightHandWeapon(Hash WeaponHash, int AmmoAmount)
	{
		QUEUE_JOB(=)
		{
			if (!WEAPON::_IS_WEAPON_ONE_HANDED(WeaponHash))
			{
				LOG_TO_CONSOLE("%s: Weapon 0x%X (%s) is not two handed!\n", __FUNCTION__, WeaponHash, HUD::GET_STRING_FROM_HASH_KEY(WeaponHash));
				return;
			}

			WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, true, false,
				WEAPON_ATTACH_POINT_HAND_SECONDARY, true, 0.5f, 1.0f, ADD_REASON_DEFAULT, true, 0.0f, false);
		}
		END_JOB()
	}

	void GiveShoulderWeapon(Hash WeaponHash, int AmmoAmount)
	{
		QUEUE_JOB(=)
		{
			if (!WEAPON::_IS_WEAPON_TWO_HANDED(WeaponHash))
			{
				LOG_TO_CONSOLE("%s: Weapon 0x%X (%s) is not two handed!\n", __FUNCTION__, WeaponHash, HUD::GET_STRING_FROM_HASH_KEY(WeaponHash));
				return;
			}

			WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, AmmoAmount, true, false,
				WEAPON_ATTACH_POINT_RIFLE_ALTERNATE, true, 0.5f, 1.0f, ADD_REASON_DEFAULT, true, 0.0f, false);
		}
		END_JOB()
	}

	void GiveWeapon(Hash WeaponHash)
	{
		switch (WeaponHash)
		{
		case AMMO_MOLOTOV_VOLATILE:
		case AMMO_DYNAMITE_VOLATILE:
		case AMMO_THROWING_KNIVES_IMPROVED:
		case AMMO_THROWING_KNIVES_POISON:
		case AMMO_TOMAHAWK_IMPROVED:
		case AMMO_TOMAHAWK_HOMING:
			GiveAmmo(WeaponHash);
			break;
		default:
			QUEUE_JOB(=)
			{
				WEAPON::GIVE_WEAPON_TO_PED(g_LocalPlayer.m_Entity, WeaponHash, 9999, true, false, WEAPON_ATTACH_POINT_HAND_PRIMARY, true,
					0.5f, 1.0f, ADD_REASON_DEFAULT, true, 0.0f, false);
			}
			END_JOB()
			break;
		}
	}

	void RemoveWeapon(Hash WeaponHash)
	{
		QUEUE_JOB(=)
		{
			WEAPON::REMOVE_WEAPON_FROM_PED(g_LocalPlayer.m_Entity, WeaponHash, true, REMOVE_REASON_DEFAULT);
		}
		END_JOB()
	}

	void RGBElectricLantern()
	{
		if (GetCurrentWeapon() != WEAPON_MELEE_LANTERN_ELECTRIC)
			return;
		
		Entity WeapEnt = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(g_LocalPlayer.m_Entity, WEAPON_ATTACH_POINT_HAND_PRIMARY);
		
		GRAPHICS::_SET_LIGHTS_TYPE_FOR_ENTITY(WeapEnt, 1);
		GRAPHICS::_SET_LIGHTS_INTENSITY_FOR_ENTITY(WeapEnt, 5.0f);
		GRAPHICS::_SET_LIGHTS_COLOR_FOR_ENTITY(WeapEnt, GET_RGB());
		GRAPHICS::UPDATE_LIGHTS_ON_ENTITY(WeapEnt);

		GRAPHICS::_SET_LIGHTS_TYPE_FOR_ENTITY(g_LocalPlayer.m_Entity, 1);
		GRAPHICS::_SET_LIGHTS_INTENSITY_FOR_ENTITY(g_LocalPlayer.m_Entity, 5.0f);
		GRAPHICS::_SET_LIGHTS_COLOR_FOR_ENTITY(g_LocalPlayer.m_Entity, GET_RGB());
		GRAPHICS::UPDATE_LIGHTS_ON_ENTITY(g_LocalPlayer.m_Entity);
	}
}
