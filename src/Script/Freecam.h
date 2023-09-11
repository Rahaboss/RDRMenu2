#pragma once

#include "pch.h"

namespace Script
{
	void CreateFreecam();
	void TickFreecam();
	void DestroyFreecam();

	inline bool FreecamEnabled = false;
}
