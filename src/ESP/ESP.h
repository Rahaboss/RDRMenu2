#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace ESP
{
	void RenderLineArray(const std::vector<ImVec2>& vec, ImU32 Color, float Thickness);
	bool RenderPedBoneESP(Ped ped);
	bool RenderTextOnEntity(Entity ent, const char* Text);
	void RenderText(const char* Text, ImVec2 Pos, ImU32 Color = IM_COL32_WHITE);
	void RenderTextCentered(const char* Text, ImVec2 Pos, ImU32 Color = IM_COL32_WHITE);
	void RenderAnimalESP();
	void RenderObjectESP();
	void RenderPedESP();
	void RenderPickupESP();
	void RenderLocalPlayerESP();
	void RenderVehicleESP();
	void RenderESP();
}
