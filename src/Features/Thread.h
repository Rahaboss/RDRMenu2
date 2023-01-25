#pragma once

#include "pch.h"
#include "Rage/joaat.h"

namespace Features
{
	void ExecuteAsThread(joaat_t ScriptHash, void (*Function)());
	void RunJobQueue();
	void YieldThread();
}
