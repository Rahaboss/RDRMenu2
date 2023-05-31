#pragma once

#include "pch.h"
#include "Menu/Clothing.h"
#include "Menu/Cutscene.h"
#include "Menu/Debug.h"
#include "Menu/Inventory.h"
#include "Menu/Logger.h"
#include "Menu/Memory.h"
#include "Menu/Overlay.h"
#include "Menu/Player.h"
#include "Menu/Rendering.h"
#include "Menu/Spawning.h"
#include "Menu/Teleport.h"
#include "Menu/Weapon.h"
#include "Menu/World.h"

namespace Menu
{
	inline bool IsOpen = false;

	void RenderMenu();
}
