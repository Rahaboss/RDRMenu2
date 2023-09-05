#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void GiveGoldCores(Ped ped);
	void FillCores(Ped ped);
	bool DoesPedBoneExist(Ped ped, int boneId);
	Vector3 GetPedBoneCoords(Ped ped, int boneId);
}
