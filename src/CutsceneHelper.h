#pragma once

#include "pch.h"
#include "Rage/natives.h"

class CutsceneHelper
{
public:
	CutsceneHelper() = delete;
	CutsceneHelper(const nlohmann::json& JsonObject);
	CutsceneHelper(const char* animDict);

	AnimScene CreateCutscene();

	void AddPedExisting(Ped Handle, const char* entityName);
	Ped AddPedNew(Hash Model, const char* entityName);
	void AddPedFromPedJson(const nlohmann::json& PedJsonObject);
	void AddPeds();
	void AddLocalPlayer();

	void AddObjectExisting(Object Handle, const char* entityName);
	void AddObjectNew(Hash Model, const char* entityName);
	void AddObjects();

	bool IsCutsceneValid() const;

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
	std::vector<Object> m_Objects;
	bool m_Loaded = false;
};
