#pragma once

#include "pch.h"

namespace Features
{
	void PlayDinoLadyCutscene();
	void PlayFishCollectorCutscene();
	void PlayIndustryCutscene();
	void PlayAnnesburgBreakoutCutscene();
	void PlayCharlesLeavingCutscene();
	void PlayJackCutscene();
	void PlayDebtCollectorCutscene();
	void PlayCutsceneFromJson(const nlohmann::json& JsonObject);
}
