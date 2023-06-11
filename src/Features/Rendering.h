#pragma once

#include "pch.h"
#include "rage/natives.h"

namespace Features
{
	void RenderText(const char* Text, float PosX, float Posy, uint8_t ColR = 255, uint8_t ColG = 255, uint8_t ColB = 255, uint8_t ColA = 255, float Scale = 1.0f);
	void RenderTextOnEntity(Entity Ent, const char* Text);
	bool WorldToScreen(float X, float Y, float Z, float& OutX, float& OutY);
	bool WorldToScreen(const Vector3& WorldPos, float& ScreenX, float& ScreenY);
	bool GetPedBoneScreenCoords(Ped ped, int boneId, ImVec2& OutCoords);
	bool RenderBoneESP(Ped ped);
	void RenderLineArray(const std::vector<ImVec2>& vec, ImU32 Color, float Thickness);
	void RenderBoneDebug(Ped ped);
	void RenderImGuiESP();

	void RenderHumanESP();
	bool RenderTextESP(Ped ped, bool ModelESP = false, bool HealthESP = false);
	bool RenderBoxESP(Ped ped);
	bool GetBoxCoords(Ped ped, ImVec2& TopLeft, ImVec2& BottomRight);
	void RenderEntityPoolDebugESP();
}
