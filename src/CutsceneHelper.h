#pragma once

#include "pch.h"
#include "Rage/natives.h"

class CutsceneHelper
{
public:
	CutsceneHelper() = delete;
	CutsceneHelper(const nlohmann::json& JsonObject);
	CutsceneHelper(const char* animDict);

	void AddPedExisting(Ped Handle, const char* entityName);
	void AddPedNew(Hash Model, const char* entityName);
	void AddPeds();
	void AddLocalPlayer();

	bool IsCutsceneValid();

	void TeleportToOrigin();
	void LoadCutscene();
	void PlayCutscene();
	void SkipCutscene();
	// [!] WILL PAUSE EVERYTHING ELSE MENU-WIDE (JOB QUEUES AND SCRIPT THREAD)
	void WaitForCutsceneEnd();
	void CleanupCutscene();
	// Loads, teleports, plays, waits and cleans up
	// Recommended to use
	void PlayAutomatically();

private:
	nlohmann::json m_JsonObject;
	AnimScene m_Scene;
	std::vector<Ped> m_Peds;
	bool m_Loaded = false;
};
