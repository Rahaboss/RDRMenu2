#pragma once

#include "pch.h"
#include "features/Cutscene.h"
#include "features/Debug.h"
#include "features/Input.h"
#include "features/Inventory.h"
#include "features/Ped.h"
#include "features/Player.h"
#include "features/Rendering.h"
#include "features/Spawning.h"
#include "features/Weapons.h"
#include "features/World.h"
#include "rage/joaat.h"
#include "rage/natives.h"
#include "rage/enums.h"

namespace Features
{
	void ExecuteAsThread(joaat_t ScriptHash, void (*Function)());
	void Setup();
	void OnTick();
	void RunJobQueue();
	void YieldThread();
	void RGBTick();

	inline uint8_t g_rgb[3]{ 255, 0, 0 };
}
