#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void RenderText(const char* Text, float PosX, float PosY, uint8_t ColR = 255, uint8_t ColG = 255, uint8_t ColB = 255, uint8_t ColA = 255, float Scale = 1.0f);
	void DisableHUD(bool Toggle);
}
