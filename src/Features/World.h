#pragma once

#include "pch.h"
#include "Rage/Natives.h"
#include "Rage/enums.h"

namespace Features
{
	void RevealMap();
	void SetClockTime(int h = 12, int m = 0, int s = 0);
	void SetSnowType(int Type);
	void SetSnowType(eSnowCoverageType Type);
	void SetWeatherType(Hash Type);
	void SetWeatherType(eWeatherType Type);
	void SetNoonAndSunny();
}
