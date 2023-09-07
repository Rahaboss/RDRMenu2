#include "pch.h"
#include "RGB.h"

static float s_RGBf[3]{ 1.0f, 0, 0 };
static uint8_t s_RGB[3]{ 255, 0, 0 };
void Renderer::RGBTick()
{
	static bool Increasing = true;
	static int iC = 1, dC = 0;

	// Change increasing color (iC) and decreasing color (dC)
	if (Increasing && s_RGBf[iC] >= 1.0f)
	{
		Increasing = false;
		iC++;
		if (iC == 3)
			iC = 0;
	}
	else if (s_RGBf[dC] <= 0.0f)
	{
		Increasing = true;
		dC++;
		if (dC == 3)
			dC = 0;
	}

	// Increase and decrease values
	constexpr float Speed = 0.5f;
	if (Increasing)
	{
		s_RGBf[iC] += ImGui::GetIO().DeltaTime * Speed;
		s_RGBf[iC] = std::clamp(s_RGBf[iC], 0.0f, 1.0f);
	}
	else
	{
		s_RGBf[dC] -= ImGui::GetIO().DeltaTime * Speed;
		s_RGBf[dC] = std::clamp(s_RGBf[dC], 0.0f, 1.0f);
	}

	s_RGB[0] = (uint8_t)(s_RGBf[0] * 255.0f);
	s_RGB[1] = (uint8_t)(s_RGBf[1] * 255.0f);
	s_RGB[2] = (uint8_t)(s_RGBf[2] * 255.0f);
}

const uint8_t* Renderer::GetRGB()
{
	return s_RGB;
}

ImVec4 Renderer::GetImGuiRGBA(float a)
{
	return ImVec4(s_RGBf[0], s_RGBf[1], s_RGBf[2], a);
}

ImU32 Renderer::GetImGuiRGBA32(uint32_t a)
{
	return IM_COL32(s_RGB[0], s_RGB[1], s_RGB[2], a);
}
