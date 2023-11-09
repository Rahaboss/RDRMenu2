#include "pch.h"
#include "Inventory.h"
#include "Rage/natives.h"
#include "Rage/Guid.h"
#include "Config/Lists.h"
#include "Util/String.h"

bool Script::GiveInventoryItem(Hash ItemHash, Hash ItemSlot, int InventoryID, Hash AddReason)
{
	Guid<4> guid1;
	Guid<5> guid2;
	Guid<5> dummy;

	std::string ItemName = Lists::GetHashName(ItemHash);
	if (!Util::IsStringValid(ItemName))
		ItemName = std::to_string(ItemHash);

	if (!INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(InventoryID, dummy.get(), RAGE_JOAAT("CHARACTER"), RAGE_JOAAT("SLOTID_NONE"), guid2.get()))
	{
		LOG_TEXT(__FUNCTION__": Failed get dummy guid %s.", ItemName.c_str());
		return false;
	}
	guid2[4] = ItemSlot;

	// Could return false but still work
	if (!INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(InventoryID, guid2.get(), ItemHash, guid2[4], guid1.get()))
	{
		//LOG_TEXT(__FUNCTION__": Failed get item guid %s.", ItemName.c_str());
		//return false;
	}

	if (!INVENTORY::_INVENTORY_ADD_ITEM_WITH_GUID(InventoryID, guid1.get(), guid2.get(), ItemHash, guid2[4], 1, AddReason))
	{
		LOG_TEXT(__FUNCTION__": Failed add item %s.", ItemName.c_str());
		return false;
	}

	return true;
}
