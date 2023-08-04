#pragma once

#include "pch.h"
#include "Rage/Natives.h"
#include "Rage/fwBasePool.h"

namespace Features
{
	void* GetBlipCollection();
	void PrintNativeHandlerAddress(uint64_t hash);

	bool IsStringValid(const char* String);
	bool IsStringValid(const std::string& String);
	void CreateConfigPath();
	std::filesystem::path GetConfigPath();
	void SetClipboardText(const char* Text);
	void SetClipboardText(const std::string& Text);

	Hash GetHashFromJson(const json& JsonObject);

	std::vector<Ped> GetAllPeds(bool CheckEntityValidity = false);
	std::vector<Object> GetAllObjects(bool CheckEntityValidity = false);
	std::vector<Vehicle> GetAllVehicles(bool CheckEntityValidity = false);
	std::vector<Pickup> GetAllPickups(bool CheckEntityValidity = false);

	void StringToLower(std::string& String);
}
