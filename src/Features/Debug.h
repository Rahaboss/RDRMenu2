#pragma once

#include "pch.h"
#include "Rage/natives.h"

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

	void* GetPedPool();
	std::vector<Ped> GetAllPeds();
}
