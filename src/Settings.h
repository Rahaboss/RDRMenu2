#pragma once

#include "pch.h"

inline nlohmann::json g_Settings;

namespace Settings
{
	void Create();
	void Destroy();
}
