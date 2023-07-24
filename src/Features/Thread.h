#pragma once

#include "pch.h"
#include "Rage/joaat.h"

namespace Features
{
	void ExecuteAsThread(rage::joaat_t ScriptHash, void (*Function)());
	void RunJobQueue();
	void YieldThread(uint32_t DurationMS = 0);
	void RunScriptCleanupAndExit();
}
