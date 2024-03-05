#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace ESP
{
	// Returns true if coordinate on screen. X and Y between [0, 1)
	bool WorldToScreen(const Vector3& WorldPos, float& ScreenX, float& ScreenY);
	const ImVec2& GetScreenSize();
	// Returns true if coordinate on screen. X and Y scaled to screen resolution
	bool WorldToScreenScaled(const Vector3& WorldPos, float& ScreenX, float& ScreenY);
	bool GetPedBoneScreenCoords(Ped ped, int boneId, ImVec2& OutCoords);
	bool GetPedBoneScreenCoordsScaled(Ped ped, int boneId, ImVec2& OutCoords);
}
