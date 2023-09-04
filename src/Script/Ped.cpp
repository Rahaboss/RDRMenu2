#include "pch.h"
#include "Ped.h"
#include "Rage/natives.h"

void Script::GiveGoldCores(Ped ped)
{
	for (int i = 0; i < 3; i++)
	{
		constexpr float Duration = 10000.0f; // seconds
		constexpr int CoreValue = 100; // 0 - 100
		constexpr BOOL Sound = false;

		ATTRIBUTE::DISABLE_ATTRIBUTE_OVERPOWER(ped, i);
		ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(ped, i, CoreValue);
		ATTRIBUTE::_ENABLE_ATTRIBUTE_CORE_OVERPOWER(ped, i, Duration, Sound);
		ATTRIBUTE::ENABLE_ATTRIBUTE_OVERPOWER(ped, i, Duration, Sound);
	}
}

void Script::FillCores(Ped ped)
{
	for (int i = 0; i < 3; i++)
		ATTRIBUTE::_SET_ATTRIBUTE_CORE_VALUE(ped, i, 100);
}
