#include "pch.h"
#include "World.h"
#include "Features.h"
#include "JobQueue.h"

namespace Features
{
	void RevealMap()
	{
		MAP::SET_MINIMAP_HIDE_FOW(TRUE);
		MAP::_REVEAL_MINIMAP_FOW(0);
	}

	void SetClockTime(int h, int m, int s)
	{
		if (h < 0 || h > 23 || m < 0 || m > 59 || s < 0 || s > 59)
			return;

		QUEUE_JOB(h, m, s)
		{
			CLOCK::SET_CLOCK_TIME(h, m, s);
		}
		END_JOB()
	}

	void SetSnowType(int type)
	{
		GRAPHICS::_SET_SNOW_COVERAGE_TYPE(type);
	}
}
