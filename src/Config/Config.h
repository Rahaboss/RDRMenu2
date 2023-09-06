#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Config
{
	void Create();
	std::filesystem::path GetConfigPath();
	Hash HashFromJSON(const json& j);
}
