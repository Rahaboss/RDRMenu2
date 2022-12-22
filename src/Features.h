#pragma once

#include "pch.h"
#include "features/Debug.h"
#include "features/Input.h"
#include "features/Inventory.h"
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
	inline bool
		EnableNoSnipers = true,
		EnableInfiniteAmmo = true,
		EnableNoSliding = true,
		EnableGodMode = true,
		EnableSuperJump = false,
		EnableNoBlackBorders = false,
		EnableNeverWanted = false;
	inline uint8_t g_rgb[3]{ 255, 0, 0 };

	void ExecuteAsThread(joaat_t script_hash, void (*function)());
	void Setup();
	void OnTick();
	void RunJobQueue();
	void YieldThread();
	void RGBTick();
}
