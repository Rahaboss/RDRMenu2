#pragma once

#include "pch.h"
#include "rage/joaat.h"

namespace Features
{
	void ExecuteAsThread(rage::joaat_t ScriptHash, void (*Function)());
	void Setup();
}
