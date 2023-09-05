#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace ESP
{
	void RenderLineArray(const std::vector<ImVec2>& vec, ImU32 Color, float Thickness);
	bool RenderPedBoneESP(Ped ped);
}
