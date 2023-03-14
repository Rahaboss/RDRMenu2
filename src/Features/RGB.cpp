#include "pch.h"
#include "RGB.h"

namespace Features
{
	static uint8_t g_rgb[3]{ 255, 0, 0 };

	void RGBTick()
	{
		constexpr int Speed = 2;
		for (int i = 0; i < Speed; i++)
		{
			static bool Increasing = true;
			static int iC = 1, dC = 0;

			// Change increasing color (iC) and decreasing color (dC)
			if (Increasing && g_rgb[iC] == 255)
			{
				Increasing = false;
				iC++;
				if (iC == 3)
					iC = 0;
			}
			else if (g_rgb[dC] == 0)
			{
				Increasing = true;
				dC++;
				if (dC == 3)
					dC = 0;
			}

			// Increase and decrease values
			if (Increasing)
				g_rgb[iC]++;
			else
				g_rgb[dC]--;
		}
	}

	const uint8_t* GetRGB()
	{
		return g_rgb;
	}

	ImVec4 GetImGuiRGB()
	{
		const auto& RGB = GetRGB();
		return ImVec4(RGB[0] / 255.0f, RGB[1] / 255.0f, RGB[2] / 255.0f, 1.0f);
	}
	
	ImU32 GetImGuiRGB32()
	{
		const auto& RGB = GetRGB();
		return IM_COL32((int)RGB[0], (int)RGB[1], (int)RGB[2], 255);
	}
}
