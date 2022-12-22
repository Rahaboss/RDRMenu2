#pragma once

#include "pch.h"
#include "rage/enums.h"

namespace Features
{
	void RevealMap();
	void SetClockTime(int h = 12, int m = 0, int s = 0);
	void SetSnowType(int type);
}

const inline std::map<std::string, int> g_SnowTypeList{
	{ "Disabled", eSnowCoverageType::Primary },
	{ "Xmas", eSnowCoverageType::Xmas },
	{ "XmasSecondary", eSnowCoverageType::XmasSecondary },
};

const inline std::map<std::string, Hash> g_WeatherTypeList{
	{ "BLIZZARD", BLIZZARD },
	{ "CLOUDS", CLOUDS },
	{ "DRIZZLE", DRIZZLE },
	{ "FOG", FOG },
	{ "GROUNDBLIZZARD", GROUNDBLIZZARD },
	{ "HAIL", HAIL },
	{ "HIGHPRESSURE", HIGHPRESSURE },
	{ "HURRICANE", HURRICANE },
	{ "MISTY", MISTY },
	{ "OVERCAST", OVERCAST },
	{ "OVERCASTDARK", OVERCASTDARK },
	{ "RAIN", RAIN },
	{ "SANDSTORM", SANDSTORM },
	{ "SHOWER", SHOWER },
	{ "SLEET", SLEET },
	{ "SNOW", SNOW },
	{ "SNOWLIGHT", SNOWLIGHT },
	{ "SUNNY", SUNNY },
	{ "THUNDER", THUNDER },
	{ "THUNDERSTORM", THUNDERSTORM },
	{ "WHITEOUT", WHITEOUT },
	{ "SNOWCLEARING", SNOWCLEARING },
};
