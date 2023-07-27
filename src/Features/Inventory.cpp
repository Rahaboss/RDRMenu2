#include "pch.h"
#include "Inventory.h"
#include "Features.h"
#include "JobQueue.h"
#include "Lists.h"

namespace Features
{
	Hash GetInventoryItemType(Hash ItemHash)
	{
		TRY
		{
			Any ItemInfo[3 * 2]{};
			if (!ITEMDATABASE::ITEMDATABASE_FILLOUT_ITEM_INFO(ItemHash, ItemInfo))
				return 0;
			return static_cast<Hash>(ItemInfo[2 * 2]);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return 0;
	}

	Hash GetInventorySlot(Hash ItemHash)
	{
		Hash ItemSlot = 0;

		TRY
		{
			switch (GetInventoryItemType(ItemHash))
			{
			case RAGE_JOAAT("CLOTHING"):
			case RAGE_JOAAT("WEAPON"):
			case RAGE_JOAAT("HORSE"):
			case RAGE_JOAAT("EMOTE"):
			case RAGE_JOAAT("UPGRADE"):
			default:
				if (INVENTORY::_INVENTORY_FITS_SLOT_ID(ItemHash, RAGE_JOAAT("SLOTID_SATCHEL")))
					ItemSlot = RAGE_JOAAT("SLOTID_SATCHEL");
				else if (INVENTORY::_INVENTORY_FITS_SLOT_ID(ItemHash, RAGE_JOAAT("SLOTID_WARDROBE")))
					ItemSlot = RAGE_JOAAT("SLOTID_WARDROBE");
				else if (INVENTORY::_INVENTORY_FITS_SLOT_ID(ItemHash, RAGE_JOAAT("SLOTID_CURRENCY")))
					ItemSlot = RAGE_JOAAT("SLOTID_CURRENCY");
				else
					ItemSlot = INVENTORY::_GET_DEFAULT_ITEM_SLOT_INFO(ItemHash, RAGE_JOAAT("CHARACTER"));
				break;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return ItemSlot;
	}

	void GiveAgedPirateRum()
	{
		GiveSingleInventoryItem(RAGE_JOAAT("CONSUMABLE_AGED_PIRATE_RUM"), RAGE_JOAAT("SLOTID_SATCHEL"), 1, ADD_REASON_DEFAULT);
	}

	void GiveAllConsumables()
	{
		QUEUE_JOB(=)
		{
			for (const auto& c : g_ConsumableList)
				GiveInventoryItem(c.second, 99);
		}
		END_JOB()
	}

	void GiveAllDocuments()
	{
		QUEUE_JOB(=)
		{
			for (const auto& d : g_DocumentList)
				GiveInventoryItem(d.second, 99);
		}
		END_JOB()
	}

	void GiveAllItemRequests()
	{
		QUEUE_JOB(=)
		{
			// Abigail
			AddMoney(500); // $5

			// Jack
			GiveInventoryItem(RAGE_JOAAT("PROVISION_JACKS_THIMBLE")); // Thimble

			GiveInventoryItem(RAGE_JOAAT("DOCUMENT_BOOK_BOY_NEW_YORK")); // Otis Miller and The Boy from New York

			// Javier
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_HERB_OLEANDER_SAGE")); // Oleander Sage

			// Mary-Beth
			GiveInventoryItem(RAGE_JOAAT("PROVISION_MARYS_FOUNTAIN_PEN")); // Fountain Pen

			// Pearson
			GiveInventoryItem(RAGE_JOAAT("PROVISION_PEARSONS_NAVAL_COMPASS"));
			
			GiveInventoryItem(RAGE_JOAAT("PROVISION_ANIMAL_CARCASS_RABBIT_HIGH_QUALITY"));

			// Sean
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_WHISKEY"));

			// Bill
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_HAIR_GREASE"));
			
			// Charles
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_MOONSHINE"));
			
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_HERB_OLEANDER_SAGE"));
			
			GiveInventoryItem(RAGE_JOAAT("PROVISION_EAGLE_FEATHER"));

			// Dutch
			GiveInventoryItem(RAGE_JOAAT("PROVISION_COMP_DUTCH_PIPE"));

			// Hosea
			GiveInventoryItem(RAGE_JOAAT("DOCUMENT_BOOK_SHREW"));
			
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_HERB_AMERICAN_GINSENG, 2"));

			// Lenny
			GiveInventoryItem(RAGE_JOAAT("PROVISION_POCKET_WATCH_SILVER"));

			// Grimshaw
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_HERB_OREGANO, 2"));

			// Tilly
			GiveInventoryItem(RAGE_JOAAT("PROVISION_NECKLACE_GOLD"));

			// Kieran
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_HERB_BURDOCK_ROOT, 2"));

			// Molly
			GiveInventoryItem(RAGE_JOAAT("PROVISION_MOLLYS_POCKET_MIRROR"));

			// Sadie
			GiveInventoryItem(RAGE_JOAAT("PROVISION_SADIES_HARMONICA"));

			// Uncle
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_HERB_MILKWEED"));
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_WHISKEY"));
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_PEPPERMINT"));
			GiveInventoryItem(RAGE_JOAAT("CONSUMABLE_HAIR_GREASE"));
			GiveInventoryItem(RAGE_JOAAT("PROVISION_STRINGY_MEAT"));
		}
		END_JOB()
	}

	void GiveAllProvisions()
	{
		QUEUE_JOB(=)
		{
			for (const auto& p : g_ProvisionList)
				GiveInventoryItem(p.second, 99);
		}
		END_JOB()
	}

	void GiveCivilWarHat()
	{
		//int guid1[5*2];
		//int guid2[4*2];
		//
		//guid1[0 * 2] = 0x80000000i32; // -2147483648 (fix warning)
		//guid1[1 * 2] = 0;
		//guid1[2 * 2] = -1678926914;
		//guid1[3 * 2] = -1554986044;
		//guid1[4 * 2] = 0;
		//
		//guid2[0 * 2] = 0x80000000i32; // -2147483648 (fix warning)
		//guid2[1 * 2] = 0;
		//guid2[2 * 2] = -1678926914;
		//guid2[3 * 2] = 2122960569;
		//
		//INVENTORY::_INVENTORY_ADD_ITEM_WITH_GUID(1, guid1, guid2, 2772348781, 2884296223, 1, ADD_REASON_DEFAULT);
		//INVENTORY::_INVENTORY_ARE_LOCAL_CHANGES_ALLOWED(1);

		constexpr Hash ItemHash = 2134589549;// RAGE_JOAAT("CLOTHING_SP_CIVIL_WAR_HAT_000_1");
		const Hash ItemSlot = 2884296223;
		//const int ItemSlot2 = 2884296223; //-1410671073
		if (!INVENTORY::_INVENTORY_FITS_SLOT_ID(ItemHash, ItemSlot))
			LOG_TO_CONSOLE("CLOTHING_SP_CIVIL_WAR_HAT_000_1 error\n");

		GiveSingleInventoryItem(ItemHash, ItemSlot, 1, ADD_REASON_DEFAULT);

		//INVENTORY::_INVENTORY_ADD_ITEM_WITH_GUID(1, 0, 0, 2435553656, 2884296223, 1, 752097756);
		/*
		Real
		_INVENTORY_ADD_ITEM_WITH_GUID(1, 00000093126FFA00, 00000093126FF9D8, 2772348781, 2884296223, 1, 752097756)
			Returned 0

			guid1:
			0
			0
			0
			0

			guid2:
			-2147483648
			0
			-491496727
			-1156075982
			-1410671073

		Expected
		_INVENTORY_ADD_ITEM_WITH_GUID(1, 000001D00A449940, 000001D00A449918, 2772348781, 2884296223, 1, 752097756)
			Returned 1

			guid1:
			-2147483648
			0
			-1678926914
			115975676 / 390940990

			guid2:
			-2147483648
			0
			-491496727
			-1156075972
			-1410671073
		*/
	}

	void GiveGinsengElixir()
	{
		TRY
		{
			GiveSingleInventoryItem(RAGE_JOAAT("CONSUMABLE_GINSENG_ELIXIER"), RAGE_JOAAT("SLOTID_SATCHEL"), 1, ADD_REASON_DEFAULT);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	// Seems to work on items like consumables, provisions, documents but not clothing
	void GiveInventoryItem(Hash ItemHash, int Amount)
	{
		for (int i = 0; i < Amount; i++)
			GiveSingleInventoryItem(ItemHash);
	}

	void GiveSingleInventoryItem(Hash ItemHash, Hash ItemSlot, int InventoryID, Hash AddReason)
	{
		Any guid1[4 * 2]{};
		Any guid2[5 * 2]{};
		Any dummy[5 * 2]{};

		if (!INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(InventoryID, dummy, RAGE_JOAAT("CHARACTER"), RAGE_JOAAT("SLOTID_NONE"), guid2))
		{
			//return;
		}
		guid2[4 * 2] = ItemSlot;

		// Could return false but still work
		if (!INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(InventoryID, guid2, ItemHash, guid2[4 * 2], guid1))
		{
			//return;
		}

		if (!INVENTORY::_INVENTORY_ADD_ITEM_WITH_GUID(InventoryID, guid1, guid2, ItemHash, guid2[4 * 2], 1, AddReason))
			return;

		//YieldThread();
	}

	void GiveValerianRoot()
	{
		TRY
		{
			GiveSingleInventoryItem(RAGE_JOAAT("CONSUMABLE_VALERIAN_ROOT"), RAGE_JOAAT("SLOTID_SATCHEL"), 1, ADD_REASON_DEFAULT);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
