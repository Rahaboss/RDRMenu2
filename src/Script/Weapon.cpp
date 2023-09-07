#include "pch.h"
#include "Weapon.h"
#include "Rage/natives.h"
#include "Menu/Menu.h"
#include "PlayerInfo.h"
#include "Rage/enums.h"
#include "Math.h"

Hash Script::GetCurrentWeapon()
{
	Hash CurWeap = 0;
	WEAPON::GET_CURRENT_PED_WEAPON(g_LocalPlayer.m_Entity, &CurWeap, false, WEAPON_ATTACH_POINT_HAND_PRIMARY, false);
	return CurWeap;
}

void Script::RapidFire()
{
	if (Menu::IsOpen)
		return;

	Hash CurrentWeapon = GetCurrentWeapon();
	if (CurrentWeapon == RAGE_JOAAT("WEAPON_UNARMED"))
		return;

	static Hash s_AimInputs[]{
		INPUT_ACCURATE_AIM,
		INPUT_AIM,
		INPUT_AIM_IN_AIR,
		INPUT_CLAIM_GENERIC,
		INPUT_HORSE_AIM,
		INPUT_VEH_AIM,
		INPUT_VEH_BOAT_AIM,
		INPUT_VEH_CAR_AIM,
		INPUT_VEH_DRAFT_AIM,
		INPUT_VEH_PASSENGER_AIM,
	};

	bool IsAiming = false;
	for (auto Input : s_AimInputs)
	{
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, Input))
		{
			IsAiming = true;
			break;
		}
	}

	if (!IsAiming)
		return;

	static Hash s_AttackInputs[]{
		INPUT_ATTACK,
		INPUT_ATTACK2,
		INPUT_HORSE_ATTACK,
		INPUT_HORSE_ATTACK2,
		INPUT_VEH_ATTACK,
		INPUT_VEH_ATTACK2,
		INPUT_VEH_BOAT_ATTACK,
		INPUT_VEH_BOAT_ATTACK2,
		INPUT_VEH_CAR_ATTACK,
		INPUT_VEH_CAR_ATTACK2,
		INPUT_VEH_DRAFT_ATTACK,
		INPUT_VEH_DRAFT_ATTACK2,
		INPUT_VEH_FLY_ATTACK,
		INPUT_VEH_FLY_ATTACK2,
		INPUT_VEH_FLY_ATTACK_CAMERA,
		INPUT_VEH_PASSENGER_ATTACK,
	};

	for (auto Input : s_AttackInputs)
		PAD::DISABLE_CONTROL_ACTION(0, Input, true);

	for (auto Input : s_AttackInputs)
	{
		if (PAD::IS_DISABLED_CONTROL_PRESSED(0, Input))
		{
			Vector3 direction = RotationToDirection(CAM::GET_GAMEPLAY_CAM_ROT(0));
			Vector3 start = CAM::GET_GAMEPLAY_CAM_COORD() + direction;
			Vector3 end = start + (direction * 200);

			// Shoot twice
			for (int i = 0; i < 2; i++)
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(start.x, start.y, start.z, end.x, end.y, end.z, 100, true, CurrentWeapon, g_LocalPlayer.m_Entity, true, false, -1.0f, false);

			return;
		}
	}
}
