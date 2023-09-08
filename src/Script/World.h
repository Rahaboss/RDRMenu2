#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void NoonAndSunny();
	void DisablePinkertonPatrols();
	bool LoadGround(Vector3 pos); // Pass by copy is intentional
	void SetWeather(Hash Weather);
	void SetSnow(int Snow);
	void SetClock(int Hour, int Minute = 0, int Second = 0);
	void ClearWanted();
}
