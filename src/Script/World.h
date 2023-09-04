#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	void NoonAndSunny();
	void DisablePinkertonPatrols();
	bool LoadGround(Vector3 pos); // Pass by copy is intentional
}
