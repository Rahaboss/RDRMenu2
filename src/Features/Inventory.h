#pragma once

#include "pch.h"
#include "Rage/natives.h"
#include "Rage/enums.h"

namespace Features
{
	Hash GetInventoryItemType(Hash ItemHash);
	Hash GetInventorySlot(Hash ItemHash);
	void GiveAgedPirateRum(); // Stamina XP
	void GiveAllConsumables();
	void GiveAllDocuments();
	void GiveAllItemRequests();
	void GiveAllProvisions();
	void GiveCivilWarHat();
	void GiveGinsengElixir(); // Health XP
	void GiveInventoryItem(Hash ItemHash, int Amount = 1);
	void GiveSingleInventoryItem(Hash ItemHash, Hash ItemSlot = 1084182731, int InventoryID = 1, Hash AddReason = ADD_REASON_DEFAULT);
	void GiveValerianRoot(); // Dead Eye XP
}
