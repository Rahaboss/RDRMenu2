#include "pch.h"
#include "World.h"
#include "Rage/natives.h"
#include "Rage/ScriptGlobal.h"
#include "Thread/Thread.h"
#include "Player.h"

void Script::NoonAndSunny()
{
	SetClock(12);
	SetWeather(RAGE_JOAAT("SUNNY"));
}

void Script::DisablePinkertonPatrols()
{
	if (GetDefaultPlayerModel() == RAGE_JOAAT("PLAYER_ZERO"))
		ScriptGlobal(1934266).At(56).Get<bool&>() = true;
}

bool Script::LoadGround(Vector3 pos)
{
	float groundZ;
	const uint8_t attempts = 10;

	for (uint8_t i = 0; i < attempts; i++)
	{
		// Only request a collision after the first try failed because the location might already be loaded on first attempt.
		for (uint16_t z = 0; i && z < 1000; z += 100)
		{
			STREAMING::REQUEST_COLLISION_AT_COORD(pos.x, pos.y, (float)z);

			Thread::YieldThread();
		}

		if (MISC::GET_GROUND_Z_FOR_3D_COORD(pos.x, pos.y, 1000.f, &groundZ, false))
		{
			pos.z = groundZ + 1.f;

			return true;
		}

		Thread::YieldThread();
	}

	return false;
}

void Script::SetWeather(Hash Weather)
{
	MISC::SET_WEATHER_TYPE(Weather, true, true, false, 0.0f, false);
}

void Script::SetSnow(int Snow)
{
	GRAPHICS::_SET_SNOW_COVERAGE_TYPE(Snow);
}

void Script::SetClock(int Hour, int Minute, int Second)
{
	if (Hour >= 0 && Hour < 24 && Minute >= 0 && Minute < 60 && Second >= 0 && Second < 60)
		CLOCK::SET_CLOCK_TIME(Hour, Minute, Second);
}
