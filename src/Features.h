#pragma once

#include "pch.h"
#include "rage/joaat.h"

namespace Features
{
	void ExecuteAsThread(joaat_t script_hash, void (*function)());
	void OnTick();
	int FpsTick();
}
