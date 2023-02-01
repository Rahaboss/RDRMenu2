#pragma once

#include "pch.h"

namespace Features
{
	void PlayDinoLadyCutscene();
	void PlayFishCollectorCutscene(int index = 0);
	void PlayIndustryCutscene();
	void PlayAnnesburgBreakoutCutscene();
	void PlayCharlesLeavingCutscene();
	void PlayJackCutscene();
	void PlayCutsceneFromJson(const nlohmann::json& JsonObject);
}
