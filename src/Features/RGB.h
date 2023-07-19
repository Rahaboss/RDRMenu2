#pragma once

#include "pch.h"

namespace Features
{
	void RGBTick();
	const uint8_t* GetRGB();
	ImVec4 GetImGuiRGB();
	ImU32 GetImGuiRGB32();
}

#define GET_RGB() Features::GetRGB()[0], Features::GetRGB()[1], Features::GetRGB()[2]
