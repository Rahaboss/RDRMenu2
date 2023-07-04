#pragma once

#include "pch.h"
#include "Rage/natives.h"
#include "Rage/fwBasePool.h"

namespace Features
{
	void* GetBlipCollection();
	void PrintNativeHandlerAddress(uint64_t hash);
	void UnlockSPPreorderBonus();

	bool IsStringValid(const char* String);
	bool IsStringValid(const std::string& String);
	std::filesystem::path GetConfigPath();
	void SetClipboardText(const char* Text);
	void SetClipboardText(const std::string& Text);

	Hash GetHashFromJson(const nlohmann::json& JsonObject);

	std::vector<Ped> GetAllPeds(bool CheckEntityValidity = false);
	std::vector<Object> GetAllObjects(bool CheckEntityValidity = false);
	std::vector<Vehicle> GetAllVehicles(bool CheckEntityValidity = false);
	std::vector<Pickup> GetAllPickups(bool CheckEntityValidity = false);
}
