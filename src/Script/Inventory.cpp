#include "pch.h"
#include "Inventory.h"
#include "Rage/natives.h"
#include "Rage/Guid.h"
#include "Config/Lists.h"
#include "Util/String.h"
#include "Script/PlayerInfo.h"

bool Script::GiveInventoryItem(Hash ItemHash)
{
	const std::string ItemName = Lists::GetHashNameOrUint(ItemHash);

	if (ItemHash == 0 || !ITEMDATABASE::_ITEMDATABASE_IS_KEY_VALID(ItemHash, 0))
	{
		LOG_TEXT(__FUNCTION__": Couldn't add item %s.", ItemName.c_str());
		return false;
	}

	const int InventoryID = INVENTORY::_INVENTORY_GET_INVENTORY_ID_FROM_PED(g_LocalPlayer.m_Entity);

	Guid<5> guid1, dummy;
	INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(InventoryID, dummy.get(), RAGE_JOAAT("CHARACTER"), RAGE_JOAAT("SLOTID_NONE"), guid1.get());
	guid1[4] = RAGE_JOAAT("SLOTID_SATCHEL");

	Guid<4> guid2;
	INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(InventoryID, guid1.get(), ItemHash, guid1[4], guid2.get());

	if (!INVENTORY::_INVENTORY_ADD_ITEM_WITH_GUID(InventoryID, guid2.get(), guid1.get(), ItemHash, guid1[4], 1, ADD_REASON_DEFAULT))
	{
		LOG_TEXT(__FUNCTION__": Couldn't add item %s.", ItemName.c_str());
		return false;
	}

	return true;
}
