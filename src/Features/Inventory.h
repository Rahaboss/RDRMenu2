#pragma once

#include "pch.h"
#include "Rage/Natives.h"

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
	void GiveSingleInventoryItem(Hash ItemHash, Hash ItemSlot = rage::joaat("SLOTID_SATCHEL"), int InventoryID = 1, Hash AddReason = rage::joaat("ADD_REASON_DEFAULT"));
	void GiveValerianRoot(); // Dead Eye XP
}
