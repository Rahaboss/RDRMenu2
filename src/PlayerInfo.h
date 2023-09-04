#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"
#include "Rage/CPed.h"

struct PlayerInfo
{
	Player m_Index;
	Ped m_Entity;
	Ped m_Mount;
	Vehicle m_Vehicle;
	Hash m_Model;
	rage::CPed* m_Ped;
};

inline PlayerInfo g_LocalPlayer{};
