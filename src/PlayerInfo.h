#pragma once

#include "pch.h"
#include "rage/natives.h"

struct PlayerInfo
{
	Player m_Index;
	Ped m_Entity;
	Ped m_Mount;
	Vehicle m_Vehicle;
} inline g_LocalPlayer;
