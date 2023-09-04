#include "pch.h"
#include "World.h"
#include "Rage/natives.h"
#include "Rage/ScriptGlobal.h"

void Script::NoonAndSunny()
{
	CLOCK::SET_CLOCK_TIME(12, 0, 0);
	MISC::SET_WEATHER_TYPE(RAGE_JOAAT("SUNNY"), true, true, false, 0.0f, false);
}

void Script::DisablePinkertonPatrols()
{
	ScriptGlobal(1934266).At(56).Get<bool&>() = true;
}
