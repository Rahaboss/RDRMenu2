#pragma once

#include "pch.h"
#include "rage/natives.h"
#include "rage/CPed.h"

struct PlayerInfo
{
	Player m_Index;
	Ped m_Entity;
	Ped m_Mount;
	Vehicle m_Vehicle;
	CPed* m_Ped;
} inline g_LocalPlayer;
