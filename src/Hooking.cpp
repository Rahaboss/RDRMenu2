#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Console.h"
#include "Features.h"
#include "Fiber.h"
#include "PlayerInfo.h"
#include "Renderer.h"
#include "Menu.h"

namespace Hooking
{
	static bool Enabled = false;

	void Create()
	{
		std::cout << "Creating hooks.\n";
		assert(MH_Initialize() == MH_OK);

		RunScriptThreads.Create(Pointers::RunScriptThreads, RunScriptThreadsHook);
		//RunScriptThreads2.Create(Pointers::RunScriptThreads2, RunScriptThreadsHook2);
		ShootBullet.Create(g_NativeContext.GetHandler(0x867654CBC7606F2C), ShootBulletHook);
		IsEntityInArea.Create(g_NativeContext.GetHandler(0xD3151E53134595E5), IsEntityInAreaHook);
#if ENABLE_ANTI_ANTI_DEBUG
		DebuggerCheck1.Create(Pointers::DebuggerCheck1, DebuggerCheck1Hook);
		//DebuggerCheck2.Create(Pointers::DebuggerCheck2, DebuggerCheck2Hook);
		if (HMODULE mod = GetModuleHandle(L"kernel32.dll"))
			IsDebuggerPresent.Create(GetProcAddress(mod, "IsDebuggerPresent"), IsDebuggerPresentHook);
#endif
		DecreaseAmmo.Create(Pointers::DecreaseAmmo, DecreaseAmmoHook);
		CreatePed.Create(g_NativeContext.GetHandler(0xD49F9B0955C367DE), CreatePedHook);
		CreateVehicle.Create(g_NativeContext.GetHandler(0xAF35D0D2583051B0), CreateVehicleHook);
		InventoryAddItem.Create(g_NativeContext.GetHandler(0xCB5D11F9508A928D), InventoryAddItemHook);
		GetGUIDFromItemID.Create(g_NativeContext.GetHandler(0x886DFD3E185C8A89), GetGUIDFromItemIDHook);
	}

	void Destroy()
	{
		std::cout << "Destroying hooks.\n";

		GetGUIDFromItemID.Destroy();
		InventoryAddItem.Destroy();
		CreateVehicle.Destroy();
		CreatePed.Destroy();
		DecreaseAmmo.Destroy();
#if ENABLE_ANTI_ANTI_DEBUG
		IsDebuggerPresent.Destroy();
		//DebuggerCheck2.Destroy();
		DebuggerCheck1.Destroy();
#endif
		IsEntityInArea.Destroy();
		ShootBullet.Destroy();
		//RunScriptThreads2.Destroy();
		RunScriptThreads.Destroy();

		assert(MH_Uninitialize() == MH_OK);
	}

	void Enable()
	{
		std::cout << "Enabling hooks.\n";
		if (Enabled)
			return;
		assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);
		Enabled = true;
	}

	void Disable()
	{
		std::cout << "Disabling hooks.\n";
		if (!Enabled)
			return;
		assert(MH_DisableHook(MH_ALL_HOOKS) == MH_OK);
		Enabled = false;
	}

	bool RunScriptThreadsHook(pgPtrCollection* this_, uint32_t ops)
	{
		TRY
		{
			if (g_Running)
			{
				constexpr joaat_t main_hash = RAGE_JOAAT("main");
				Features::ExecuteAsThread(main_hash, ScriptThreadTick);
				
				return RunScriptThreads.GetOriginal<decltype(&RunScriptThreadsHook)>()(this_, ops);
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return false;
	}

	//bool RunScriptThreadsHook2(uint32_t ops)
	//{
	//	TRY
	//	{
	//		if (g_running)
	//		{
	//			constexpr joaat_t main_hash = RAGE_JOAAT("main");
	//			Features::ExecuteAsThread(main_hash, ScriptThreadTick);
	//		
	//			return RunScriptThreads2.GetOriginal<decltype(&RunScriptThreadsHook2)>()(ops);
	//		}
	//	}
	//	EXCEPT{ LOG_EXCEPTION(); }
	//
	//	return false;
	//}

	void ShootBulletHook(scrNativeCallContext* ctx)
	{
		TRY
		{
			if (Features::EnableNoSnipers && ctx->GetArg<Hash>(8) == WEAPON_SNIPERRIFLE_CARCANO)
				return;
			
			ShootBullet.GetOriginal<decltype(&ShootBulletHook)>()(ctx);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	BOOL IsEntityInAreaHook(scrNativeCallContext* ctx)
	{
		TRY
		{
			if (Features::EnableNoSnipers && ctx->GetArg<Entity>(0) == g_LocalPlayer.m_Entity &&
				ctx->GetArg<uint32_t>(1) == 0x44BBD654) // 1502.69775391f
			{
				*(BOOL*)ctx->m_ReturnValue = FALSE; // spoof return value
				return FALSE;
			}

			return IsEntityInArea.GetOriginal<decltype(&IsEntityInAreaHook)>()(ctx);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return FALSE;
	}

#if ENABLE_ANTI_ANTI_DEBUG
	void DebuggerCheck1Hook(uint32_t a1)
	{
		std::cout << __FUNCTION__"(" << a1 << ")\n";
		return;
	}

	void DebuggerCheck2Hook(int32_t a1, int32_t a2, int32_t a3)
	{
		std::cout << __FUNCTION__"(" << a1 << ", " << a2 << ", " << a3 << ")\n";
		return;
	}

	BOOL WINAPI IsDebuggerPresentHook()
	{
		std::cout << __FUNCTION__"()\n";
		return FALSE;
	}
#endif

	void DecreaseAmmoHook(uint64_t a1, CPed* a2, uint64_t a3, uint32_t a4)
	{
		TRY
		{
			if (Features::EnableInfiniteAmmo && a2 == Pointers::GetPlayerPed(g_LocalPlayer.m_Index))
				return;

			Hooking::DecreaseAmmo.GetOriginal<decltype(&DecreaseAmmoHook)>()(a1, a2, a3, a4);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	Ped CreatePedHook(scrNativeCallContext* ctx)
	{
		Ped result = 0;

		TRY
		{
			if (!(g_Settings["log_human_spawning"].get_ref<bool&>()) && !(g_Settings["log_ped_spawning"].get_ref<bool&>()))
			{
				result = Hooking::CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);
			}
			else if (ctx)
			{
				Hash model = ctx->GetArg<Hash>(0);
				Vector3 pos = ctx->GetArg<Vector3>(1);

				result = Hooking::CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);
				Ped id = ctx->GetRet<Ped>();

				if (PED::IS_PED_HUMAN(id))
				{
					if (g_Settings["log_human_spawning"].get_ref<bool&>())
					{
						//std::cout << "Creating human " << Features::GetPedModelName(model) << " (" << LOG_HEX(model)
						//	<< ") ID: " << LOG_HEX(id) << " at: " << pos << ".\n";

						Menu::Logger.AddLog("Creating human %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, id,
							pos.x, pos.y, pos.z);
					}
				}
				else if (g_Settings["log_ped_spawning"].get_ref<bool&>())
				{
					//std::cout << "Creating ped " << Features::GetPedModelName(model) << " (" << LOG_HEX(model)
					//	<< ") ID: " << LOG_HEX(id) << " at: " << pos << ".\n";

					Menu::Logger.AddLog("Creating ped %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, id,
						pos.x, pos.y, pos.z);
				}
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
	
	Vehicle CreateVehicleHook(scrNativeCallContext* ctx)
	{
		Vehicle result = 0;

		TRY
		{
			if (ctx && g_Settings["log_vehicle_spawning"].get_ref<bool&>())
			{
				Hash model = ctx->GetArg<Hash>(0);
				Vector3 pos = ctx->GetArg<Vector3>(1);

				result = Hooking::CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);
				Vehicle id = ctx->GetRet<Vehicle>();

				//std::cout << "Creating vehicle " << Features::GetVehicleModelName(model) << " (" << LOG_HEX(model)
				//	<< ") ID: " << LOG_HEX(id) << " at: " << pos << ".\n";

				Menu::Logger.AddLog("Creating vehicle %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, id,
					pos.x, pos.y, pos.z);
			}
			else
			{
				result = Hooking::CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
	
	BOOL InventoryAddItemHook(scrNativeCallContext* ctx)
	{
		BOOL result = 0;

		TRY
		{
			if (ctx && g_Settings["log_added_inventory_items"].get_ref<bool&>())
			{
				int inventoryId = ctx->GetArg<int>(0);
				Any* guid1 = ctx->GetArg<Any*>(1);
				Any* guid2 = ctx->GetArg<Any*>(2);
				Hash item = ctx->GetArg<Hash>(3);
				Hash inventoryItemSlot = ctx->GetArg<Hash>(4);
				int p5 = ctx->GetArg<int>(5);
				Hash addReason = ctx->GetArg<Hash>(6);

				result = Hooking::InventoryAddItem.GetOriginal<decltype(&InventoryAddItemHook)>()(ctx);
				BOOL ret = ctx->GetRet<BOOL>();

				constexpr Hash CLOTHING_SP_CIVIL_WAR_HAT_000_1 = RAGE_JOAAT("CLOTHING_SP_CIVIL_WAR_HAT_000_1");
				switch (inventoryId)
				{
#if ENABLE_LARGE_STACK_ITEMS
				case CONSUMABLE_BIG_GAME_MEAT_COOKED:
				case CONSUMABLE_BIG_GAME_MEAT_OREGANO_COOKED:
				case CONSUMABLE_BIG_GAME_MEAT_THYME_COOKED:
				case CONSUMABLE_BIG_GAME_MEAT_WILD_MINT_COOKED:
#endif
				case CLOTHING_SP_CIVIL_WAR_HAT_000_1:
				default:
					Menu::Logger.AddLog("_INVENTORY_ADD_ITEM_WITH_GUID(%d, 0x%llX, 0x%llX, %d, %d, %d, %d)\n",
						inventoryId, guid1, guid2, item, inventoryItemSlot, p5, addReason);
					Menu::Logger.AddLog("\tReturned %d\n\n", ret);
					Menu::Logger.AddLog("\tguid1:\n");
					for (int i = 0; i < 4; i++)
						Menu::Logger.AddLog("\t%d\n", ((int*)guid1)[i * 2]);
					Menu::Logger.AddLog("\tguid2:\n");
					for (int i = 0; i < 5; i++)
						Menu::Logger.AddLog("\t%d\n", ((int*)guid2)[i * 2]);
					break;
				}

				//std::cout << "_INVENTORY_ADD_ITEM_WITH_GUID(" << inventoryId << ", " << guid1 << ", " << guid2 << ", " << item << ", "
				//	<< inventoryItemSlot << ", " << p5 << ", " << addReason << ")\n";
				//std::cout << "\tReturned " << ret << "\n\n";
				//
				//std::cout << "\tguid1:\n";
				//for (int i = 0; i < 4; i++)
				//	std::cout << "\t" << ((int*)guid1)[i * 2] << "\n";
				//
				//std::cout << "\n\tguid2:\n";
				//for (int i = 0; i < 5; i++)
				//	std::cout << "\t" << ((int*)guid2)[i * 2] << "\n";
			}
			else
			{
				result = Hooking::InventoryAddItem.GetOriginal<decltype(&InventoryAddItemHook)>()(ctx);
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
	
	BOOL GetGUIDFromItemIDHook(scrNativeCallContext* ctx)
	{
		BOOL result = 0;

		TRY
		{
			if (ctx && g_Settings["log_added_inventory_items"].get_ref<bool&>())
			{
				int inventoryId = ctx->GetArg<int>(0);
				Any* guid = ctx->GetArg<Any*>(1);
				Hash p2 = ctx->GetArg<Hash>(2);
				Hash slotId = ctx->GetArg<Hash>(3);
				Any* outGuid = ctx->GetArg<Any*>(4);

				result = Hooking::GetGUIDFromItemID.GetOriginal<decltype(&GetGUIDFromItemIDHook)>()(ctx);
				BOOL ret = ctx->GetRet<BOOL>();

				constexpr Hash CLOTHING_SP_CIVIL_WAR_HAT_000_1 = RAGE_JOAAT("CLOTHING_SP_CIVIL_WAR_HAT_000_1");
				switch (inventoryId)
				{
#if ENABLE_LARGE_STACK_ITEMS
				case CONSUMABLE_BIG_GAME_MEAT_COOKED:
				case CONSUMABLE_BIG_GAME_MEAT_OREGANO_COOKED:
				case CONSUMABLE_BIG_GAME_MEAT_THYME_COOKED:
				case CONSUMABLE_BIG_GAME_MEAT_WILD_MINT_COOKED:
#endif
				case CLOTHING_SP_CIVIL_WAR_HAT_000_1:
					Menu::Logger.AddLog("_INVENTORY_ADD_ITEM_WITH_GUID(%d, %p, %u, %u, %p)\n", inventoryId, guid, p2, slotId, outGuid);
					Menu::Logger.AddLog("\tReturned %d\n\n", ret);
					Menu::Logger.AddLog("\tguid:\n");
					for (int i = 0; i < 5; i++)
						Menu::Logger.AddLog("\t%d\n", ((int*)guid)[i * 2]);
					Menu::Logger.AddLog("\toutGuid:\n");
					for (int i = 0; i < 5; i++)
						Menu::Logger.AddLog("\t%d\n", ((int*)outGuid)[i * 2]);
					break;
				}
			}
			else
			{
				result = Hooking::GetGUIDFromItemID.GetOriginal<decltype(&GetGUIDFromItemIDHook)>()(ctx);
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}

	HRESULT STDMETHODCALLTYPE SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags)
	{
		HRESULT result = S_FALSE;
		
		TRY
		{
			if (g_Running)
				Renderer::Present();

			result = Hooking::SwapChainPresent.GetOriginal<decltype(&SwapChainPresentHook)>()(SwapChain, SyncInterval, Flags);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
}
