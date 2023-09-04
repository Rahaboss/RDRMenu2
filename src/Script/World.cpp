#include "pch.h"
#include "World.h"
#include "Rage/natives.h"
#include "Rage/ScriptGlobal.h"
#include "Thread/Thread.h"

void Script::NoonAndSunny()
{
	CLOCK::SET_CLOCK_TIME(12, 0, 0);
	MISC::SET_WEATHER_TYPE(RAGE_JOAAT("SUNNY"), true, true, false, 0.0f, false);
}

void Script::DisablePinkertonPatrols()
{
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
