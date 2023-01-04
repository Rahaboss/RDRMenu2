#pragma once

#include "pch.h"
#include "rage/natives.h"
#include "rage/CPed.h"

struct PlayerInfo
{
	Player m_Index; // PLAYER::PLAYER_ID
	Ped m_Entity; // PLAYER::PLAYER_PED_ID / PLAYER::GET_PLAYER_PED_SCRIPT_INDEX
	Ped m_Mount; // PED::GET_MOUNT (0 if not on mount)
	Ped m_LastMount; // PLAYER::_GET_ACTIVE_HORSE_FOR_PLAYER / PED::_GET_LAST_MOUNT (0 if no last mount)
	Vehicle m_Vehicle; // PED::GET_VEHICLE_PED_IS_IN (0 if not in vehicle)
	Vector3 m_Pos; // ENTITY::GET_ENTITY_COORDS
	CPed* m_Ped; // CPedFactory::GetLocalPed / Pointers::GetPlayerPed
} inline g_LocalPlayer;
