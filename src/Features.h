#pragma once

#include "pch.h"
#include "Features/Cutscene.h"
#include "Features/Debug.h"
#include "Features/Input.h"
#include "Features/Inventory.h"
#include "Features/Math.h"
#include "Features/Ped.h"
#include "Features/Player.h"
#include "Features/Rendering.h"
#include "Features/RGB.h"
#include "Features/Spawning.h"
#include "Features/Thread.h"
#include "Features/Weapons.h"
#include "Features/World.h"

namespace Features
{
	void Setup();
	void Cleanup();
	void OnTick();
}
