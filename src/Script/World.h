#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void NoonAndSunny();
	void DisablePinkertonPatrols();
	bool LoadGround(Vector3& pos);
	void SetWeather(Hash Weather);
	void SetSnow(int Snow);
	void SetClock(int Hour, int Minute = 0, int Second = 0);
	void ClearWanted();
	void DisableBlackBorders();
	void SetTimeScale(float Scale);
	void RevealMap();
	float GetDistanceBetweenPoints(const Vector3& p1, const Vector3& p2);
}
