#pragma once

#include "pch.h"

namespace Renderer
{
	// FPS-independent RGB wave function

	void RGBTick();
	const uint8_t* GetRGB();
	ImVec4 GetImGuiRGBA(float a = 1.0f);
	ImU32 GetImGuiRGBA32(uint32_t a = 255);
}
