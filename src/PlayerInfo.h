#pragma once

#include "pch.h"
#include "rage/natives.h"
#include "rage/CPed.h"

struct PlayerInfo
{
	Player m_Index; // PLAYER::PLAYER_ID
	Ped m_Entity; // PLAYER::PLAYER_PED_ID / PLAYER::GET_PLAYER_PED_SCRIPT_INDEX
	Ped m_Mount; // PED::GET_MOUNT (0 if not on mount)
	Vehicle m_Vehicle; // PED::GET_VEHICLE_PED_IS_IN (0 if not in vehicle)
	CPed* m_Ped; // Pointers::GetPlayerPed
} inline g_LocalPlayer;
