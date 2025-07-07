#include "pch.h"
#include "World.h"
#include "Rage/natives.h"
#include "Rage/ScriptGlobal.h"
#include "Thread/Thread.h"
#include "Player.h"
#include "PlayerInfo.h"

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

bool Script::LoadGround(Vector3& pos)
{
	float GroundZ;
	const uint8_t Attempts = 10;

	for (uint8_t i = 0; i < Attempts; i++)
	{
		// Only request a collision after the first try failed because the location might already be loaded on first attempt
		for (uint16_t z = 0; i && z < 1000; z += 100)
		{
			STREAMING::REQUEST_COLLISION_AT_COORD(pos.x, pos.y, static_cast<float>(z));

			Thread::YieldThread();
		}

		if (MISC::GET_GROUND_Z_FOR_3D_COORD(pos.x, pos.y, 1000.0f, &GroundZ, false))
		{
			pos.z = GroundZ + 1.0f;

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

void Script::ClearWanted()
{
	LAW::SET_BOUNTY(g_LocalPlayer.m_Index, 0);
	LAW::_SET_BOUNTY_HUNTER_PURSUIT_CLEARED();
	LAW::SET_WANTED_SCORE(g_LocalPlayer.m_Index, 0);
}

void Script::DisableBlackBorders()
{
	CAM::_REQUEST_LETTER_BOX_OVERTIME(-1, -1, false, 17, true, false);
	CAM::_FORCE_LETTER_BOX_THIS_UPDATE();
}

void Script::SetTimeScale(float Scale)
{
	MISC::SET_TIME_SCALE(std::clamp(Scale, 0.0f, 1.0f));
}

void Script::RevealMap()
{
	MAP::SET_MINIMAP_HIDE_FOW(true);
	MAP::_REVEAL_MINIMAP_FOW(0);
}

float Script::GetDistanceBetweenPoints(const Vector3& p1, const Vector3& p2)
{
	Vector3 d(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);

	return sqrt(d.x * d.x + d.x * d.x + d.x * d.x);
}
