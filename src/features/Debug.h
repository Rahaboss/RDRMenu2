#pragma once

#include "pch.h"
#include "rage/natives.h"

namespace Features
{
	void* GetBlipCollection();
	void* GetScriptGlobal(uint32_t Index);
	void PrintNativeHandlerAddress(uint64_t hash);
	void UnlockSPPreorderBonus();
	void PlayDinoLadyCutscene();
	void PlayFishCollectorCutscene();
	void PlayIndustryCutscene();
}
