#pragma once

#include "pch.h"
#include "Rage/scrNativeCallContext.h"

namespace Script
{
	bool GiveInventoryItem(Hash ItemHash, Hash ItemSlot = RAGE_JOAAT("SLOTID_SATCHEL"),
		int InventoryID = 1, Hash AddReason = RAGE_JOAAT("ADD_REASON_DEFAULT"));
}
