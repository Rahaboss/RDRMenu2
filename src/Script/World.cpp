#include "pch.h"
#include "World.h"
#include "rage/natives.h"

void Script::NoonAndSunny()
{
	CLOCK::SET_CLOCK_TIME(12, 0, 0);
	MISC::SET_WEATHER_TYPE(RAGE_JOAAT("SUNNY"), true, true, false, 0.0f, false);
}
