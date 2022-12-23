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
			Any ItemInfo[3 * 2]; memset(ItemInfo, 0, sizeof(ItemInfo));
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
				if (INVENTORY::_INVENTORY_FITS_SLOT_ID(ItemHash, 1084182731))
					ItemSlot = 1084182731;
				else if (INVENTORY::_INVENTORY_FITS_SLOT_ID(ItemHash, 1034665895))
					ItemSlot = 1034665895;
				else if (INVENTORY::_INVENTORY_FITS_SLOT_ID(ItemHash, -833319691))
					ItemSlot = -833319691;
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
		GiveSingleInventoryItem(CONSUMABLE_AGED_PIRATE_RUM, 1084182731, 1, ADD_REASON_DEFAULT);
	}

	void GiveAllConsumables()
	{
		QUEUE_JOB()
		{
			for (const auto& c : g_ConsumableList)
				GiveInventoryItem(c.second, 99);
		}
		END_JOB()
	}

	void GiveAllDocuments()
	{
		QUEUE_JOB()
		{
			for (const auto& d : g_DocumentList)
				GiveInventoryItem(d.second, 99);
		}
		END_JOB()
	}

	void GiveAllItemRequests()
	{
		QUEUE_JOB()
		{
			// Abigail
			AddMoney(500); // $5

			// Jack
			GiveInventoryItem(PROVISION_JACKS_THIMBLE); // Thimble

			GiveInventoryItem(DOCUMENT_BOOK_BOY_NEW_YORK); // Otis Miller and The Boy from New York

			// Javier
			GiveInventoryItem(CONSUMABLE_HERB_OLEANDER_SAGE); // Oleander Sage

			// Mary-Beth
			GiveInventoryItem(PROVISION_MARYS_FOUNTAIN_PEN); // Fountain Pen

			// Pearson
			GiveInventoryItem(PROVISION_PEARSONS_NAVAL_COMPASS);
			
			GiveInventoryItem(PROVISION_ANIMAL_CARCASS_RABBIT_HIGH_QUALITY);

			// Sean
			GiveInventoryItem(CONSUMABLE_WHISKEY);

			// Bill
			GiveInventoryItem(CONSUMABLE_HAIR_GREASE);
			
			// Charles
			GiveInventoryItem(CONSUMABLE_MOONSHINE);
			
			GiveInventoryItem(CONSUMABLE_HERB_OLEANDER_SAGE);
			
			GiveInventoryItem(PROVISION_EAGLE_FEATHER);

			// Dutch
			GiveInventoryItem(PROVISION_COMP_DUTCH_PIPE);

			// Hosea
			GiveInventoryItem(DOCUMENT_BOOK_SHREW);
			
			GiveInventoryItem(CONSUMABLE_HERB_AMERICAN_GINSENG, 2);

			// Lenny
			GiveInventoryItem(PROVISION_POCKET_WATCH_SILVER);

			// Grimshaw
			GiveInventoryItem(CONSUMABLE_HERB_OREGANO, 2);

			// Tilly
			GiveInventoryItem(PROVISION_NECKLACE_GOLD);

			// Kieran
			GiveInventoryItem(CONSUMABLE_HERB_BURDOCK_ROOT, 2);

			// Molly
			GiveInventoryItem(PROVISION_MOLLYS_POCKET_MIRROR);

			// Sadie
			GiveInventoryItem(PROVISION_SADIES_HARMONICA);

			// Uncle
			GiveInventoryItem(CONSUMABLE_HERB_MILKWEED);
			GiveInventoryItem(CONSUMABLE_WHISKEY);
			GiveInventoryItem(CONSUMABLE_PEPPERMINT);
			GiveInventoryItem(CONSUMABLE_HAIR_GREASE);
			GiveInventoryItem(PROVISION_ANIMAL_CARCASS_SKUNK_HIGH_QUALITY);
		}
		END_JOB()
	}

	void GiveAllProvisions()
	{
		QUEUE_JOB()
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
		{
			std::cout << "CLOTHING_SP_CIVIL_WAR_HAT_000_1 error\n";
		}

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
			GiveSingleInventoryItem(CONSUMABLE_GINSENG_ELIXIER, 1084182731, 1, ADD_REASON_DEFAULT);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void GiveInventoryItem(Hash ItemHash, int Amount)
	{
		for (int i = 0; i < Amount; i++)
			GiveSingleInventoryItem(ItemHash);
	}

	// Seems to work on consumables but not clothing
	void GiveSingleInventoryItem(Hash ItemHash, Hash ItemSlot, int InventoryID, Hash AddReason)
	{
		Any guid1[4 * 2]; memset(guid1, 0, sizeof(guid1));
		Any guid2[5 * 2]; memset(guid2, 0, sizeof(guid2));
		Any dummy[5 * 2]; memset(dummy, 0, sizeof(dummy));

		bool fail1 = false;
		if (!INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(InventoryID, dummy, RAGE_JOAAT("CHARACTER"), 0xA1212100 /* -1591664384 */, guid2))
		{
			//return;
		}
		guid2[4 * 2] = ItemSlot;

		bool fail2 = false;
		// Could return false but still work
		if (!INVENTORY::INVENTORY_GET_GUID_FROM_ITEMID(InventoryID, guid2, ItemHash, guid2[4 * 2], guid1))
		{
			//return;
		}

		if (!INVENTORY::_INVENTORY_ADD_ITEM_WITH_GUID(InventoryID, guid1, guid2, ItemHash, guid2[4 * 2], 1, AddReason))
			return;

		YieldThread();
	}

	void GiveValerianRoot()
	{
		TRY
		{
			GiveSingleInventoryItem(CONSUMABLE_VALERIAN_ROOT, 1084182731, 1, ADD_REASON_DEFAULT);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
