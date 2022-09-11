#pragma once

#include "pch.h"
#include "rage/joaat.h"

namespace Features
{
	void ExecuteAsThread(joaat_t script_hash, void (*function)());
	void Setup();
	void OnTick();
	void RunJobQueue();
	int FpsTick();
	void YieldThread();
}
