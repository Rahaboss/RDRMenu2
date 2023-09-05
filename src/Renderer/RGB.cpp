#include "pch.h"
#include "RGB.h"

static uint8_t s_RGB[3]{ 255, 0, 0 };
void Renderer::RGBTick()
{
	constexpr int Speed = 2;
	for (int i = 0; i < Speed; i++)
	{
		static bool Increasing = true;
		static int iC = 1, dC = 0;

		// Change increasing color (iC) and decreasing color (dC)
		if (Increasing && s_RGB[iC] == 255)
		{
			Increasing = false;
			iC++;
			if (iC == 3)
				iC = 0;
		}
		else if (s_RGB[dC] == 0)
		{
			Increasing = true;
			dC++;
			if (dC == 3)
				dC = 0;
		}

		// Increase and decrease values
		if (Increasing)
			s_RGB[iC]++;
		else
			s_RGB[dC]--;
	}
}

const uint8_t* Renderer::GetRGB()
{
	return s_RGB;
}

ImVec4 Renderer::GetImGuiRGB()
{
	const uint8_t* RGB = GetRGB();
	return ImVec4(RGB[0] / 255.0f, RGB[1] / 255.0f, RGB[2] / 255.0f, 1.0f);
}

ImU32 Renderer::GetImGuiRGB32()
{
	const uint8_t* RGB = GetRGB();
	return IM_COL32((int)RGB[0], (int)RGB[1], (int)RGB[2], 255);
}
