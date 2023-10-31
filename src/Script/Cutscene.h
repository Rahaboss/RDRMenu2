#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	class CutsceneHelper
	{
	public:
		CutsceneHelper() = delete;
		CutsceneHelper(const json& JsonObject);
		CutsceneHelper(const char* animDict);

		AnimScene CreateCutscene();

		void AddPedExisting(Ped Handle, const char* entityName);
		Ped AddPedNew(Hash Model, const char* entityName);
		void AddPedFromPedJson(const json& PedJsonObject);
		void AddPeds();
		void AddLocalPlayer();

		void AddObjectExisting(Object Handle, const char* entityName);
		void AddObjectNew(Hash Model, const char* entityName);
		void AddObjects();

		void AddVehicleExisting(Vehicle Handle, const char* entityName);
		void AddVehicleFromVehicleJson(const json& VehicleJsonObject);
		void AddVehicleNew(Hash Model, const char* entityName);
		void AddVehicles();

		bool IsCutsceneValid() const;

		void SetOrigin();
		void TeleportToOrigin();
		void LoadCutscene();
		void PlayCutscene();
		void SkipCutscene();
		void WaitForCutsceneEnd();
		void CleanupCutscene();
		// Loads, teleports, plays, waits and cleans up. Recommended to use
		void PlayAutomatically();

		AnimScene GetCutscene() const;

	private:
		json m_JsonObject;
		AnimScene m_Scene;
		std::vector<Object> m_Objects;
		std::vector<Ped> m_Peds;
		std::vector<Vehicle> m_Vehicles;
	};

	void AddEntityToCutscene(const char* CutsceneName, Entity ent, const char* EntityName);
	void AddEntityPlaybackID(const char* CutsceneName, const char* PlaybackID);
	bool IsCutsceneName(std::string CutsceneName);
}
