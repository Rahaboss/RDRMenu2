#pragma once

#include "pch.h"
#include "rage/natives.h"

namespace Features
{
	void* GetBlipCollection();
	CPedFactory* GetPedFactory();
	void PrintNativeHandlerAddress(const uint64_t& hash);
	void UnlockSPPreorderBonus();
	void PlayDinoLadyCutscene();
	void PlayFishCollectorCutscene();
	void PlayIndustryCutscene();
}
