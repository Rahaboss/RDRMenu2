#include "pch.h"
#include "Features.h"

namespace Features
{
	void RevealMap()
	{
		MAP::SET_MINIMAP_HIDE_FOW(TRUE);
		MAP::_REVEAL_MINIMAP_FOW(0);
	}

	void SetSnowType(eSnowCoverageType type)
	{
		GRAPHICS::_SET_SNOW_COVERAGE_TYPE(type);
	}
}
