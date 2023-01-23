#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Features.h"
#include "Fiber.h"
#include "PlayerInfo.h"
#include "Renderer.h"
#include "Menu.h"
#include "Lists.h"

namespace Hooking
{
	static bool Enabled = false;

	void Create()
	{
		printf("Creating hooks.\n");
		assert(MH_Initialize() == MH_OK);

		RunScriptThreads.Create(Pointers::RunScriptThreads, RunScriptThreadsHook);
		//RunScriptThreads2.Create(Pointers::RunScriptThreads2, RunScriptThreadsHook2);
		ShootBullet.Create(NativeContext::GetHandler(0x867654CBC7606F2C), ShootBulletHook);
		IsEntityInArea.Create(NativeContext::GetHandler(0xD3151E53134595E5), IsEntityInAreaHook);
#if ENABLE_ANTI_ANTI_DEBUG
		DebuggerCheck1.Create(Pointers::DebuggerCheck1, DebuggerCheck1Hook);
		//DebuggerCheck2.Create(Pointers::DebuggerCheck2, DebuggerCheck2Hook);
		if (HMODULE mod = GetModuleHandle(L"kernel32.dll"))
			IsDebuggerPresent.Create(GetProcAddress(mod, "IsDebuggerPresent"), IsDebuggerPresentHook);
#endif
		DecreaseAmmo.Create(Pointers::DecreaseAmmo, DecreaseAmmoHook);
		CreatePed.Create(NativeContext::GetHandler(0xD49F9B0955C367DE), CreatePedHook);
		CreateVehicle.Create(NativeContext::GetHandler(0xAF35D0D2583051B0), CreateVehicleHook);
		InventoryAddItem.Create(NativeContext::GetHandler(0xCB5D11F9508A928D), InventoryAddItemHook);
		GetGUIDFromItemID.Create(NativeContext::GetHandler(0x886DFD3E185C8A89), GetGUIDFromItemIDHook);
		CreatePersChar.Create(NativeContext::GetHandler(0x4F76E3676583D951), CreatePersCharHook);
		CreateAnimScene.Create(NativeContext::GetHandler(0x1FCA98E33C1437B3), CreateAnimSceneHook);
		DecorSetBool.Create(NativeContext::GetHandler(0xFE26E4609B1C3772), DecorSetIntHook);
		DecorSetInt.Create(NativeContext::GetHandler(0xE88F4D7F52A6090F), DecorSetIntHook);
	}

	void Destroy()
	{
		printf("Destroying hooks.\n");

		DecorSetInt.Destroy();
		DecorSetBool.Destroy();
		CreateAnimScene.Destroy();
		CreatePersChar.Destroy();
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
		printf("Enabling hooks.\n");
		if (Enabled)
			return;
		assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);
		Enabled = true;
	}

	void Disable()
	{
		printf("Disabling hooks.\n");
		if (!Enabled)
			return;
		assert(MH_DisableHook(MH_ALL_HOOKS) == MH_OK);
		Enabled = false;
	}

	bool RunScriptThreadsHook(pgPtrCollection* this_, uint32_t ops)
	{
		TRY
		{
			bool Result = RunScriptThreads.GetOriginal<decltype(&RunScriptThreadsHook)>()(this_, ops);
			
			if (g_Running)
			{
				constexpr joaat_t main_hash = RAGE_JOAAT("main");
				Features::ExecuteAsThread(main_hash, ScriptThreadTick);
			}
			
			return Result;
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
			if (g_Settings["no_snipers"].get<bool>() && ctx &&
				ctx->GetArg<Hash>(8) == WEAPON_SNIPERRIFLE_CARCANO)
				return;
			
			ShootBullet.GetOriginal<decltype(&ShootBulletHook)>()(ctx);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	BOOL IsEntityInAreaHook(scrNativeCallContext* ctx)
	{
		TRY
		{
			if (g_Settings["no_snipers"].get<bool>() && ctx &&
				ctx->GetArg<Entity>(0) == g_LocalPlayer.m_Entity &&
				ctx->GetArg<uint32_t>(1) == 0x44BBD654) // 1502.69775391f
			{
				//ctx->GetRet<BOOL>() = FALSE;
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
		printf("%s(%u)\n", __FUNCTION__, a1);
		return;
	}

	void DebuggerCheck2Hook(int32_t a1, int32_t a2, int32_t a3)
	{
		printf("%s(%d, %d, %d)\n", __FUNCTION__, a1, a2, a3);
		return;
	}

	BOOL WINAPI IsDebuggerPresentHook()
	{
		printf("%s()\n", __FUNCTION__);
		return FALSE;
	}
#endif

	void DecreaseAmmoHook(uint64_t a1, CPed* a2, uint64_t a3, uint32_t a4)
	{
		TRY
		{
			if (a2 == g_LocalPlayer.m_Ped && g_Settings["infinite_ammo"].get<bool>())
				return;

			DecreaseAmmo.GetOriginal<decltype(&DecreaseAmmoHook)>()(a1, a2, a3, a4);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	Ped CreatePedHook(scrNativeCallContext* ctx)
	{
		if (!ctx || (!g_Settings["log_human_spawning"].get<bool>() && !g_Settings["log_ped_spawning"].get<bool>()))
			return CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);

		Hash model = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);

		Ped result = CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);
		Ped ret = ctx->GetRet<Ped>();

		if (PED::IS_PED_HUMAN(ret))
		{
			if (g_Settings["log_human_spawning"].get<bool>())
			{
				Menu::Logger.AddLog("Creating human %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, ret, pos.x, pos.y, pos.z);
			}
		}
		else if (g_Settings["log_ped_spawning"].get<bool>())
		{
			Menu::Logger.AddLog("Creating ped %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, ret, pos.x, pos.y, pos.z);
		}

		return result;
	}
	
	Vehicle CreateVehicleHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["log_vehicle_spawning"].get<bool>())
			return CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);

		Hash model = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);

		Vehicle result = CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);
		Vehicle ret = ctx->GetRet<Vehicle>();

		Menu::Logger.AddLog("Creating vehicle %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetVehicleModelName(model).data(), model, ret,
			pos.x, pos.y, pos.z);

		return result;
	}
	
	BOOL InventoryAddItemHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["log_added_inventory_items"].get<bool>())
			return InventoryAddItem.GetOriginal<decltype(&InventoryAddItemHook)>()(ctx);
			
		BOOL result = 0;

		TRY
		{
			int inventoryId = ctx->GetArg<int>(0);
			Any* guid1 = ctx->GetArg<Any*>(1);
			Any* guid2 = ctx->GetArg<Any*>(2);
			Hash item = ctx->GetArg<Hash>(3);
			Hash inventoryItemSlot = ctx->GetArg<Hash>(4);
			int p5 = ctx->GetArg<int>(5);
			Hash addReason = ctx->GetArg<Hash>(6);

			result = InventoryAddItem.GetOriginal<decltype(&InventoryAddItemHook)>()(ctx);
			BOOL ret = ctx->GetRet<BOOL>();

			constexpr Hash CLOTHING_SP_CIVIL_WAR_HAT_000_1 = RAGE_JOAAT("CLOTHING_SP_CIVIL_WAR_HAT_000_1");
			switch (inventoryId)
			{
			case CONSUMABLE_BIG_GAME_MEAT_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_OREGANO_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_THYME_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_WILD_MINT_COOKED:
			case CLOTHING_SP_CIVIL_WAR_HAT_000_1:
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
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
	
	BOOL GetGUIDFromItemIDHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["log_added_inventory_items"].get<bool>())
			return GetGUIDFromItemID.GetOriginal<decltype(&GetGUIDFromItemIDHook)>()(ctx);

		BOOL result = 0;

		TRY
		{
			int inventoryId = ctx->GetArg<int>(0);
			Any* guid = ctx->GetArg<Any*>(1);
			Hash p2 = ctx->GetArg<Hash>(2);
			Hash slotId = ctx->GetArg<Hash>(3);
			Any* outGuid = ctx->GetArg<Any*>(4);

			result = GetGUIDFromItemID.GetOriginal<decltype(&GetGUIDFromItemIDHook)>()(ctx);
			BOOL ret = ctx->GetRet<BOOL>();

			constexpr Hash CLOTHING_SP_CIVIL_WAR_HAT_000_1 = RAGE_JOAAT("CLOTHING_SP_CIVIL_WAR_HAT_000_1");
			switch (inventoryId)
			{
			case CONSUMABLE_BIG_GAME_MEAT_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_OREGANO_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_THYME_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_WILD_MINT_COOKED:
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

			result = SwapChainPresent.GetOriginal<decltype(&SwapChainPresentHook)>()(SwapChain, SyncInterval, Flags);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}

	PersChar CreatePersCharHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["log_human_spawning"].get<bool>())
			return CreatePersChar.GetOriginal<decltype(&CreatePersCharHook)>()(ctx);

		Hash hash = ctx->GetArg<Hash>(0);
		PersChar result = CreatePersChar.GetOriginal<decltype(&CreatePersCharHook)>()(ctx);
		PersChar ret = ctx->GetRet<PersChar>();
		Hash model = PERSCHAR::_GET_PERSCHAR_MODEL_NAME(hash);
		if (!model)
		{
			Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(ret);
			model = ENTITY::GET_ENTITY_MODEL(ped);
		}

		if (model)
		{
			const auto it = g_PedModelNameList.find(model);
			if (it != g_PedModelNameList.end())
				Menu::Logger.AddLog("Creating persistent character %s (0x%X) hash: 0x%X, ID: 0x%X\n", it->second.data(), model, hash, ret);
		}

		return result;
	}

	AnimScene CreateAnimSceneHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["log_created_cutscenes"].get<bool>())
			return CreateAnimScene.GetOriginal<decltype(&CreateAnimSceneHook)>()(ctx);
		
		const char* animDict = ctx->GetArg<const char*>(0);
		int flags = ctx->GetArg<int>(1);
		const char* playbackListName = ctx->GetArg<const char*>(2);
		bool p3 = ctx->GetArg<BOOL>(3);
		bool p4 = ctx->GetArg<BOOL>(4);
		AnimScene result = CreateAnimScene.GetOriginal<decltype(&CreateAnimSceneHook)>()(ctx);
		AnimScene ret = ctx->GetRet<AnimScene>();

		if (std::string(animDict).find("cutscene@") != std::string::npos)
			Menu::Logger.AddLog("CREATE_ANIM_SCENE(\"%s\", %d, \"%s\", %s, %s) = %d\n", animDict, flags, playbackListName,
				(p3 ? "true" : "false"), (p4 ? "true" : "false"), ret);

		return result;
	}

	BOOL DecorSetBoolHook(scrNativeCallContext* ctx)
	{
		BOOL result = 0;

		TRY
		{
			if (!ctx || !g_Settings["log_set_decor"].get<bool>())
				return DecorSetBool.GetOriginal<decltype(&DecorSetBoolHook)>()(ctx);

			Entity entity = ctx->GetArg<Entity>(0);
			const char* propertyName = ctx->GetArg<const char*>(1);
			BOOL value = ctx->GetArg<BOOL>(2);
			result = DecorSetBool.GetOriginal<decltype(&DecorSetBoolHook)>()(ctx);
			BOOL ret = ctx->GetRet<BOOL>();

			if (entity == g_LocalPlayer.m_Entity)
				Menu::Logger.AddLog("DECORATOR::DECOR_SET_BOOL(g_LocalPlayer.m_Entity, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else if (entity == g_LocalPlayer.m_Mount)
				Menu::Logger.AddLog("DECORATOR::DECOR_SET_BOOL(g_LocalPlayer.m_Mount, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else if (entity == g_LocalPlayer.m_Vehicle)
				Menu::Logger.AddLog("DECORATOR::DECOR_SET_BOOL(g_LocalPlayer.m_Vehicle, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else
				Menu::Logger.AddLog("DECORATOR::DECOR_SET_BOOL(%u, \"%s\", %s)\n", entity, propertyName, (value ? "true" : "false"));
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}

	BOOL DecorSetIntHook(scrNativeCallContext* ctx)
	{
		BOOL result = 0;

		TRY
		{
			if (!ctx || !g_Settings["log_set_decor"].get<bool>())
				return DecorSetInt.GetOriginal<decltype(&DecorSetIntHook)>()(ctx);

			Entity entity = ctx->GetArg<Entity>(0);
			const char* propertyName = ctx->GetArg<const char*>(1);
			int value = ctx->GetArg<int>(2);
			result = DecorSetInt.GetOriginal<decltype(&DecorSetIntHook)>()(ctx);
			BOOL ret = ctx->GetRet<BOOL>();

			if (entity == g_LocalPlayer.m_Entity)
				Menu::Logger.AddLog("DECORATOR::DECOR_SET_INT(g_LocalPlayer.m_Entity, \"%s\", %d)\n", propertyName, value);
			else if (entity == g_LocalPlayer.m_Mount)
				Menu::Logger.AddLog("DECORATOR::DECOR_SET_INT(g_LocalPlayer.m_Mount, \"%s\", %d)\n", propertyName, value);
			else if (entity == g_LocalPlayer.m_Vehicle)
				Menu::Logger.AddLog("DECORATOR::DECOR_SET_INT(g_LocalPlayer.m_Vehicle, \"%s\", %d)\n", propertyName, value);
			else
				Menu::Logger.AddLog("DECORATOR::DECOR_SET_INT(%u, \"%s\", %d)\n", entity, propertyName, value);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
}
