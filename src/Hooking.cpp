#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Features.h"
#include "Fiber.h"
#include "PlayerInfo.h"
#include "Renderer.h"
#include "Menu.h"
#include "Lists.h"
#include "JobQueue.h"

namespace Hooking
{
	static bool s_Enabled = false;

	void Create()
	{
		LOG_TO_CONSOLE("Creating hooks.\n");
		assert(MH_Initialize() == MH_OK);

		DetourHook::s_HookCount = 0;
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
		SetAnimSceneEntity.Create(NativeContext::GetHandler(0x8B720AD451CA2AB3), SetAnimSceneEntityHook);
		IsDlcPresent.Create(NativeContext::GetHandler(0x2763DC12BBE2BB6F), IsDlcPresentHook);
#if !ENABLE_UNTESTED
		GetAnimScenePed.Create(NativeContext::GetHandler(0xE5822422197BBBA3), GetAnimScenePedHook);
		CreateMetapedPed.Create(NativeContext::GetHandler(0x0BCD4091C8EABA42), CreateMetapedPedHook);
		CreateMetapedOutfitPed.Create(NativeContext::GetHandler(0xEAF682A14F8E5F53), CreateMetapedOutfitPedHook);
		ClonePed.Create(NativeContext::GetHandler(0x214651FB1DFEBA89), ClonePedHook);
		ForceSpawnPersChar.Create(NativeContext::GetHandler(0x0CADC3A977997472), ForceSpawnPersCharHook);
		ForceSpawnPersChar2.Create(NativeContext::GetHandler(0x08FC896D2CB31FCC), ForceSpawnPersChar2Hook);
		//ForceSpawnPersChar3.Create(NativeContext::GetHandler(0xFCC6DB8DBE709BC8), ForceSpawnPersChar3Hook);
		//ForceSpawnPersChar4.Create(NativeContext::GetHandler(0x112DDF56300BC6E5), ForceSpawnPersChar4Hook);
#endif
		CreateObject.Create(NativeContext::GetHandler(0x509D5878EB39E842), CreateObjectHook);
		CreateObjectNoOffset.Create(NativeContext::GetHandler(0x9A294B2138ABB884), CreateObjectNoOffsetHook);
	}

	void Destroy()
	{
		LOG_TO_CONSOLE("Destroying hooks.\n");

		CreateObjectNoOffset.Destroy();
		CreateObject.Destroy();
#if !ENABLE_UNTESTED
		//ForceSpawnPersChar4.Destroy();
		//ForceSpawnPersChar3.Destroy();
		ForceSpawnPersChar2.Destroy();
		ForceSpawnPersChar.Destroy();
		ClonePed.Destroy();
		CreateMetapedOutfitPed.Destroy();
		CreateMetapedPed.Destroy();
		GetAnimScenePed.Destroy();
#endif
		IsDlcPresent.Destroy();
		SetAnimSceneEntity.Destroy();
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

		assert(DetourHook::s_HookCount = 0);
		assert(MH_Uninitialize() == MH_OK);
	}

	void Enable()
	{
		LOG_TO_CONSOLE("Enabling hooks.\n");
		if (s_Enabled)
			return;
		assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);
		s_Enabled = true;
	}

	void Disable()
	{
		LOG_TO_CONSOLE("Disabling hooks.\n");
		if (!s_Enabled)
			return;
		assert(MH_DisableHook(MH_ALL_HOOKS) == MH_OK);
		s_Enabled = false;
	}

	bool RunScriptThreadsHook(pgPtrCollection* this_, uint32_t ops)
	{
		TRY
		{
			bool Result = RunScriptThreads.GetOriginal<decltype(&RunScriptThreadsHook)>()(this_, ops);
			
			if (g_Running)
			{
				constexpr joaat_t MainHash = RAGE_JOAAT("main");
				Features::ExecuteAsThread(MainHash, ScriptThreadTick);
			}
			
			return Result;
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return false;
	}

	bool RunScriptThreadsHook2(uint32_t ops)
	{
		TRY
		{
			bool Result = RunScriptThreads2.GetOriginal<decltype(&RunScriptThreadsHook2)>()(ops);

			if (g_Running)
			{
				constexpr joaat_t MainHash = RAGE_JOAAT("main");
				Features::ExecuteAsThread(MainHash, ScriptThreadTick);
			}

			return Result;
		}
		EXCEPT{ LOG_EXCEPTION(); }
	
		return false;
	}

	void ShootBulletHook(scrNativeCallContext* ctx)
	{
		if (ctx && g_Settings["no_snipers"].get<bool>() &&
			ctx->GetArg<Hash>(8) == WEAPON_SNIPERRIFLE_CARCANO)
			return;
			
		ShootBullet.GetOriginal<decltype(&ShootBulletHook)>()(ctx);
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
		LOG_TO_CONSOLE("%s(%u)\n", __FUNCTION__, a1);
		return;
	}

	void DebuggerCheck2Hook(int32_t a1, int32_t a2, int32_t a3)
	{
		LOG_TO_CONSOLE("%s(%d, %d, %d)\n", __FUNCTION__, a1, a2, a3);
		return;
	}

	BOOL WINAPI IsDebuggerPresentHook()
	{
		LOG_TO_CONSOLE("%s()\n", __FUNCTION__);
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
		if (!ctx)
			return CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);

		Ped result = CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);

		Features::LogSpawnedEntity(ctx->GetRet<Ped>());

		return result;
	}
	
	Vehicle CreateVehicleHook(scrNativeCallContext* ctx)
	{
		if (!ctx)
			return CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);

		Vehicle result = CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);

		Features::LogSpawnedEntity(ctx->GetRet<Vehicle>());

		return result;
	}
	
	BOOL InventoryAddItemHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["added_inventory_item"].get<bool>())
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

			switch (inventoryId)
			{
			case CONSUMABLE_BIG_GAME_MEAT_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_OREGANO_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_THYME_COOKED:
			case CONSUMABLE_BIG_GAME_MEAT_WILD_MINT_COOKED:
			case RAGE_JOAAT("CLOTHING_SP_CIVIL_WAR_HAT_000_1"):
				LOG_TO_MENU("_INVENTORY_ADD_ITEM_WITH_GUID(%d, 0x%llX, 0x%llX, %d, %d, %d, %d)\n",
					inventoryId, guid1, guid2, item, inventoryItemSlot, p5, addReason);
				LOG_TO_MENU("\tReturned %d\n\n", ret);
				LOG_TO_MENU("\tguid1:\n");
				for (int i = 0; i < 4; i++)
					LOG_TO_MENU("\t%d\n", ((int*)guid1)[i * 2]);
				LOG_TO_MENU("\tguid2:\n");
				for (int i = 0; i < 5; i++)
					LOG_TO_MENU("\t%d\n", ((int*)guid2)[i * 2]);
				break;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
	
	BOOL GetGUIDFromItemIDHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["added_inventory_item"].get<bool>())
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
				LOG_TO_MENU("_INVENTORY_ADD_ITEM_WITH_GUID(%d, %p, %u, %u, %p)\n", inventoryId, guid, p2, slotId, outGuid);
				LOG_TO_MENU("\tReturned %d\n\n", ret);
				LOG_TO_MENU("\tguid:\n");
				for (int i = 0; i < 5; i++)
					LOG_TO_MENU("\t%d\n", ((int*)guid)[i * 2]);
				LOG_TO_MENU("\toutGuid:\n");
				for (int i = 0; i < 5; i++)
					LOG_TO_MENU("\t%d\n", ((int*)outGuid)[i * 2]);
				break;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}

	HRESULT STDMETHODCALLTYPE SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags)
	{
		HRESULT Result = S_FALSE;
		
		TRY
		{
			if (g_Running)
			{
				Renderer::Present();
				//LOG_TO_CONSOLE("%s(%u, %u);\n", __FUNCTION__, SyncInterval, Flags);
			}

			Result = Hooking::SwapChain.GetOriginal<decltype(&SwapChainPresentHook)>(SwapChainPresentIndex)(SwapChain, SyncInterval, Flags);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return Result;
	}

	HRESULT STDMETHODCALLTYPE SwapChainResizeBuffersHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		LOG_TO_CONSOLE("%s(0x%llX, %u, %u, %u, %d, %u)\n", __FUNCTION__, (uintptr_t)SwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

		HRESULT Result = S_FALSE;

		TRY
		{
			if (g_Running)
				ImGui_ImplDX12_InvalidateDeviceObjects();

			Result = Hooking::SwapChain.GetOriginal<decltype(&SwapChainResizeBuffersHook)>(SwapChainResizeBuffersIndex)(SwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

			if (g_Running && SUCCEEDED(Result))
				ImGui_ImplDX12_CreateDeviceObjects();
		}
		EXCEPT{ LOG_EXCEPTION(); }
		
		return Result;
	}

	PersChar CreatePersCharHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["spawned_human"].get<bool>())
			return CreatePersChar.GetOriginal<decltype(&CreatePersCharHook)>()(ctx);

		Hash hash = ctx->GetArg<Hash>(0);
		PersChar result = CreatePersChar.GetOriginal<decltype(&CreatePersCharHook)>()(ctx);
		PersChar ret = ctx->GetRet<PersChar>();
		Hash model = PERSCHAR::_GET_PERSCHAR_MODEL_NAME(hash);

		// PERSCHAR::_GET_PERSCHAR_PED_INDEX always returns 0 for some reason
#if 0
		Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(ret);
		if (!model)
			model = ENTITY::GET_ENTITY_MODEL(ped);
		
		if (PED::IS_PED_HUMAN(ped))
			Features::g_AddedPeds.push_back(ped);
#endif

		if (!model)
			return result;
		
		const auto it = g_PedModelNameList.find(model);
		if (it == g_PedModelNameList.end())
			return result;
		
		LOG_TO_MENU("Creating persistent character %s (0x%X) hash: 0x%X, ID: 0x%X\n", it->second.data(), model, hash, ret);

		QUEUE_JOB(=)
		{
			Features::YieldThread(100);

			Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(ret);
			if (ENTITY::DOES_ENTITY_EXIST(ped))
			{
				LOG_TO_MENU("Adding existing perschar %s (0x%X) to ped list\n", it->second.data(), ret);

				//if (PED::IS_PED_HUMAN(ped))
				//	Features::g_AddedPeds.push_back(ped);
			}
			else
			{
				LOG_TO_MENU("Perschar %s (0x%X) did not spawn in time!\n", it->second.data(), ret);
			}
		}
		END_JOB()

		return result;
	}

	AnimScene CreateAnimSceneHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["created_cutscene"].get<bool>())
			return CreateAnimScene.GetOriginal<decltype(&CreateAnimSceneHook)>()(ctx);
		
		const char* animDict = ctx->GetArg<const char*>(0);
		int flags = ctx->GetArg<int>(1);
		const char* playbackListName = ctx->GetArg<const char*>(2);
		bool p3 = ctx->GetArg<BOOL>(3);
		bool p4 = ctx->GetArg<BOOL>(4);
		AnimScene result = CreateAnimScene.GetOriginal<decltype(&CreateAnimSceneHook)>()(ctx);
		AnimScene ret = ctx->GetRet<AnimScene>();

		// Check if is cutscene (instead of scripted event)
		//if (std::string(animDict).find("cutscene@") == std::string::npos)
		//	return result;

		if (!playbackListName)
		{
			LOG_TO_MENU("CREATE_ANIM_SCENE(\"%s\", %d, NULL, %s, %s) = %d\n", animDict, flags,
				(p3 ? "true" : "false"), (p4 ? "true" : "false"), ret);
		}
		else
		{
			LOG_TO_MENU("CREATE_ANIM_SCENE(\"%s\", %d, \"%s\", %s, %s) = %d\n", animDict, flags, playbackListName,
				(p3 ? "true" : "false"), (p4 ? "true" : "false"), ret);
		}
		
		return result;
	}

	BOOL DecorSetBoolHook(scrNativeCallContext* ctx)
	{
		BOOL result = 0;

		TRY
		{
			if (!ctx || !g_Settings["logging"]["set_decor"].get<bool>())
				return DecorSetBool.GetOriginal<decltype(&DecorSetBoolHook)>()(ctx);

			Entity entity = ctx->GetArg<Entity>(0);
			const char* propertyName = ctx->GetArg<const char*>(1);
			BOOL value = ctx->GetArg<BOOL>(2);
			result = DecorSetBool.GetOriginal<decltype(&DecorSetBoolHook)>()(ctx);
			BOOL ret = ctx->GetRet<BOOL>();

			if (entity == g_LocalPlayer.m_Entity)
				LOG_TO_MENU("DECORATOR::DECOR_SET_BOOL(g_LocalPlayer.m_Entity, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else if (entity == g_LocalPlayer.m_Mount || entity == g_LocalPlayer.m_LastMount)
				LOG_TO_MENU("DECORATOR::DECOR_SET_BOOL(g_LocalPlayer.m_Mount, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else if (entity == g_LocalPlayer.m_Vehicle)
				LOG_TO_MENU("DECORATOR::DECOR_SET_BOOL(g_LocalPlayer.m_Vehicle, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else
				LOG_TO_MENU("DECORATOR::DECOR_SET_BOOL(%u, \"%s\", %s)\n", entity, propertyName, (value ? "true" : "false"));
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}

	BOOL DecorSetIntHook(scrNativeCallContext* ctx)
	{
		BOOL result = 0;

		TRY
		{
			if (!ctx || !g_Settings["logging"]["set_decor"].get<bool>())
				return DecorSetInt.GetOriginal<decltype(&DecorSetIntHook)>()(ctx);

			Entity entity = ctx->GetArg<Entity>(0);
			const char* propertyName = ctx->GetArg<const char*>(1);
			int value = ctx->GetArg<int>(2);
			result = DecorSetInt.GetOriginal<decltype(&DecorSetIntHook)>()(ctx);
			BOOL ret = ctx->GetRet<BOOL>();

			if (entity == g_LocalPlayer.m_Entity)
				LOG_TO_MENU("DECORATOR::DECOR_SET_INT(g_LocalPlayer.m_Entity, \"%s\", %d)\n", propertyName, value);
			else if (entity == g_LocalPlayer.m_Mount || entity == g_LocalPlayer.m_LastMount)
				LOG_TO_MENU("DECORATOR::DECOR_SET_INT(g_LocalPlayer.m_Mount, \"%s\", %d)\n", propertyName, value);
			else if (entity == g_LocalPlayer.m_Vehicle)
				LOG_TO_MENU("DECORATOR::DECOR_SET_INT(g_LocalPlayer.m_Vehicle, \"%s\", %d)\n", propertyName, value);
			else
				LOG_TO_MENU("DECORATOR::DECOR_SET_INT(%u, \"%s\", %d)\n", entity, propertyName, value);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}

	void SetAnimSceneEntityHook(scrNativeCallContext* ctx)
	{
		if (g_Settings["logging"]["added_cutscene_entity"].get<bool>())
		{
			// void SET_ANIM_SCENE_ENTITY(AnimScene animScene, const char* entityName, Entity entity, int flags)
			AnimScene animScene = ctx->GetArg<AnimScene>(0);
			const char* entityName = ctx->GetArg<const char*>(1);
			Entity entity = ctx->GetArg<Entity>(2);
			int flags = ctx->GetArg<int>(3);
			Hash model = ENTITY::GET_ENTITY_MODEL(entity);
			std::string modelStr;
			
			const auto it = g_PedModelNameList.find(model);
			if (it != g_PedModelNameList.end())
			{
				modelStr = it->second;
			}
			else
			{
				const auto it2 = g_VehicleModelNameList.find(model);
				if (it2 != g_VehicleModelNameList.end())
				{
					modelStr = it2->second;
				}
				else
				{
					if (joaat(entityName) == model)
					{
						modelStr = entityName;
					}
					else
					{
						modelStr = std::to_string(model);
					}
				}
			}

#if !ENABLE_UNTESTED
			if (entity != g_LocalPlayer.m_Entity && ENTITY::DOES_ENTITY_EXIST(entity) && PED::IS_PED_HUMAN(entity))
			{
				bool exists = false;
				for (const auto& p : Features::g_AddedPeds)
				{
					if (p == entity)
					{
						exists = true;
						break;
					}
				}

				//if (!exists)
				//	Features::g_AddedPeds.push_back(entity);
			}
#endif

			LOG_TO_MENU("Adding animscene entity %s (model: %s) entity: 0x%X to animscene %d\n", entityName, modelStr.c_str(), entity, animScene);
		}

		SetAnimSceneEntity.GetOriginal<decltype(&SetAnimSceneEntityHook)>()(ctx);
	}
	
	BOOL IsDlcPresentHook(scrNativeCallContext* ctx)
	{
		BOOL result = 0;

		TRY
		{
			Hash dlcHash = ctx->GetArg<Hash>(0);
			result = IsDlcPresent.GetOriginal<decltype(&IsDlcPresentHook)>()(ctx);
			BOOL result2 = ctx->GetRet<BOOL>();
			const char* dlcName = 0;

			switch (dlcHash)
			{
			case 3615828851u:
				ctx->GetRet<BOOL>() = TRUE;
				break;
			case joaat("DLC_PHYSPREORDERCONTENT"):
				dlcName = "DLC_PHYSPREORDERCONTENT";
				ctx->GetRet<BOOL>() = TRUE;
				break;
			case joaat("DLC_PREORDERCONTENT"):
				dlcName = "DLC_PHYSPREORDERCONTENT";
				ctx->GetRet<BOOL>() = TRUE;
				break;
			case joaat("DLC_SPECIALEDITION"):
				dlcName = "DLC_PHYSPREORDERCONTENT";
				ctx->GetRet<BOOL>() = TRUE;
				break;
			case joaat("DLC_ULTIMATEEDITION"):
				dlcName = "DLC_PHYSPREORDERCONTENT";
				ctx->GetRet<BOOL>() = TRUE;
				break;
			case joaat("XX_I$RAWKST4H_D3V_XX"): // LMAO
				ctx->GetRet<BOOL>() = TRUE;
				dlcName = "XX_I$RAWKST4H_D3V_XX";
				break;
			}

			if (dlcName)
			{
				//LOG_TO_MENU("IS_DLC_PRESENT(\"%s\") = %s\n", dlcName, (result2 ? "true" : "false"));
				return TRUE;
			}
			//else
			//	LOG_TO_MENU("IS_DLC_PRESENT(%u) = %s\n", dlcHash, (result2 ? "true" : "false"));
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
	
#if !ENABLE_UNTESTED
	Ped GetAnimScenePedHook(scrNativeCallContext* ctx)
	{
		AnimScene animScene = ctx->GetArg<AnimScene>(0);
		const char* name = ctx->GetArg<const char*>(1);
		BOOL isNetwork = ctx->GetArg<BOOL>(2);
		
		Ped result = GetAnimScenePed.GetOriginal<decltype(&GetAnimScenePedHook)>()(ctx);
		Ped ret = ctx->GetRet<Ped>();

		//if (ret == 0 || !ENTITY::DOES_ENTITY_EXIST(ret))
		//	return;
		
		LOG_TO_MENU("_GET_ANIM_SCENE_PED(%u, \"%s\", %s) = %u\n", animScene, name, (isNetwork ? "TRUE" : "FALSE"), ret);

		//Features::g_AddedPeds.push_back(result);

		return result;
	}
	
	Ped CreateMetapedPedHook(scrNativeCallContext* ctx)
	{
		if (!ctx || (!g_Settings["logging"]["spawned_human"].get<bool>() && !g_Settings["logging"]["spawned_ped"].get<bool>()))
			return CreateMetapedPed.GetOriginal<decltype(&CreateMetapedPedHook)>()(ctx);

		puts(__FUNCTION__);

		Hash model = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);

		Ped result = CreateMetapedPed.GetOriginal<decltype(&CreateMetapedPedHook)>()(ctx);
		Ped ret = ctx->GetRet<Ped>();

		if (PED::IS_PED_HUMAN(ret))
		{
			if (g_Settings["logging"]["spawned_human"].get<bool>())
			{
				LOG_TO_MENU("Creating metaped human %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, ret, pos.x, pos.y, pos.z);
			}
			//Features::g_AddedPeds.push_back(ret);
		}
		else if (g_Settings["logging"]["spawned_ped"].get<bool>())
		{
			LOG_TO_MENU("Creating metaped ped %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, ret, pos.x, pos.y, pos.z);
		}

		return result;
	}
	
	Ped CreateMetapedOutfitPedHook(scrNativeCallContext* ctx)
	{
		if (!ctx || (!g_Settings["logging"]["spawned_human"].get<bool>() && !g_Settings["logging"]["spawned_ped"].get<bool>()))
			return CreateMetapedOutfitPed.GetOriginal<decltype(&CreateMetapedOutfitPedHook)>()(ctx);
		
		Hash model = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);

		Ped result = CreateMetapedOutfitPed.GetOriginal<decltype(&CreateMetapedOutfitPedHook)>()(ctx);
		Ped ret = ctx->GetRet<Ped>();

		if (PED::IS_PED_HUMAN(ret))
		{
			if (g_Settings["logging"]["spawned_human"].get<bool>())
			{
				LOG_TO_MENU("Creating metaped outfit human %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, ret, pos.x, pos.y, pos.z);
			}
			//Features::g_AddedPeds.push_back(ret);
		}
		else if (g_Settings["logging"]["spawned_ped"].get<bool>())
		{
			LOG_TO_MENU("Creating metaped outfit ped %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, ret, pos.x, pos.y, pos.z);
		}

		return result;
	}
	
	Ped ClonePedHook(scrNativeCallContext* ctx)
	{
		if (!ctx || (!g_Settings["logging"]["spawned_human"].get<bool>() && !g_Settings["logging"]["spawned_ped"].get<bool>()))
			return ClonePed.GetOriginal<decltype(&ClonePedHook)>()(ctx);

		puts(__FUNCTION__);

		Hash model = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);

		Ped result = ClonePed.GetOriginal<decltype(&ClonePedHook)>()(ctx);
		Ped ret = ctx->GetRet<Ped>();

		if (PED::IS_PED_HUMAN(ret))
		{
			if (g_Settings["logging"]["spawned_human"].get<bool>())
			{
				LOG_TO_MENU("Creating cloned human %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, ret, pos.x, pos.y, pos.z);
			}
			//Features::g_AddedPeds.push_back(ret);
		}
		else if (g_Settings["logging"]["spawned_ped"].get<bool>())
		{
			LOG_TO_MENU("Creating cloned ped %s (0x%X) ID: 0x%X at: %.2f, %.2f, %.2f\n", Features::GetPedModelName(model).data(), model, ret, pos.x, pos.y, pos.z);
		}

		return result;
	}
	
	Entity ForceSpawnPersCharHook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["spawned_human"].get<bool>())
			return ForceSpawnPersChar.GetOriginal<decltype(&ForceSpawnPersCharHook)>()(ctx);

		PersChar persChar = ctx->GetArg<Hash>(0);
		BOOL p1 = ctx->GetArg<BOOL>(1);
		Entity result = ForceSpawnPersChar.GetOriginal<decltype(&ForceSpawnPersCharHook)>()(ctx);
		Entity ret = ctx->GetRet<PersChar>();

		Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(persChar);
		if (//(result == 0 || !ENTITY::DOES_ENTITY_EXIST(result)) && (ret == 0 || !ENTITY::DOES_ENTITY_EXIST(ret)) && (ped == 0 || !ENTITY::DOES_ENTITY_EXIST(ped))
			result == 0 && ret == 0 && ped == 0)
			return result;

		if (ret == 0)
			ret = ped;

		if (ret == 0)
			ret = result;

		Hash model = ENTITY::GET_ENTITY_MODEL(ret);

		LOG_TO_MENU(__FUNCTION__": Spawning persistent character %s (0x%X), ID: 0x%X\n", Features::GetPedModelName(model).c_str(), model, ret);

		//if (PED::IS_PED_HUMAN(ret))
		//	Features::g_AddedPeds.push_back(ret);

#if 0
		Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(ret);
		if (!model)
			model = ENTITY::GET_ENTITY_MODEL(ped);

		if (PED::IS_PED_HUMAN(ped))
			Features::g_AddedPeds.push_back(ped);

		if (model)
		{
			const auto it = g_PedModelNameList.find(model);
			if (it != g_PedModelNameList.end())
				LOG_TO_MENU("Creating persistent character %s (0x%X) hash: 0x%X, ID: 0x%X\n", it->second.data(), model, hash, ret);
		}
#endif

		return result;
	}
	
	Entity ForceSpawnPersChar2Hook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["spawned_human"].get<bool>())
			return ForceSpawnPersChar2.GetOriginal<decltype(&ForceSpawnPersChar2Hook)>()(ctx);

		PersChar persChar = ctx->GetArg<Hash>(0);
		BOOL p1 = ctx->GetArg<BOOL>(1);
		Entity result = ForceSpawnPersChar2.GetOriginal<decltype(&ForceSpawnPersChar2Hook)>()(ctx);
		Entity ret = ctx->GetRet<PersChar>();

		Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(persChar);
		if (//(result == 0 || !ENTITY::DOES_ENTITY_EXIST(result)) && (ret == 0 || !ENTITY::DOES_ENTITY_EXIST(ret)) && (ped == 0 || !ENTITY::DOES_ENTITY_EXIST(ped))
			result == 0 && ret == 0 && ped == 0)
			return result;

		if (ret == 0)
			ret = ped;

		if (ret == 0)
			ret = result;

		Hash model = ENTITY::GET_ENTITY_MODEL(ret);

		LOG_TO_MENU(__FUNCTION__": Spawning persistent character %s (0x%X), ID: 0x%X\n", Features::GetPedModelName(model).c_str(), model, ret);

		//if (PED::IS_PED_HUMAN(ret))
		//	Features::g_AddedPeds.push_back(ret);

#if 0
		Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(ret);
		if (!model)
			model = ENTITY::GET_ENTITY_MODEL(ped);

		if (PED::IS_PED_HUMAN(ped))
			Features::g_AddedPeds.push_back(ped);

		if (model)
		{
			const auto it = g_PedModelNameList.find(model);
			if (it != g_PedModelNameList.end())
				LOG_TO_MENU("Creating persistent character %s (0x%X) hash: 0x%X, ID: 0x%X\n", it->second.data(), model, hash, ret);
		}
#endif

		return result;
	}
	
	void ForceSpawnPersChar3Hook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["spawned_human"].get<bool>())
		{
			ForceSpawnPersChar3.GetOriginal<decltype(&ForceSpawnPersChar3Hook)>()(ctx);
			return;
		}

		PersChar persChar = ctx->GetArg<Hash>(0);
		BOOL p1 = ctx->GetArg<BOOL>(1);
		ForceSpawnPersChar3.GetOriginal<decltype(&ForceSpawnPersChar3Hook)>()(ctx);

		Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(persChar);
		if (ped == 0 /*|| !ENTITY::DOES_ENTITY_EXIST(ped)*/)
			return;

		Hash model = ENTITY::GET_ENTITY_MODEL(ped);

		LOG_TO_MENU(__FUNCTION__": Spawning persistent character %s (0x%X), ID: 0x%X\n", Features::GetPedModelName(model).c_str(), model, ped);

		//if (PED::IS_PED_HUMAN(ped))
		//	Features::g_AddedPeds.push_back(ped);

#if 0
		Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(ret);
		if (!model)
			model = ENTITY::GET_ENTITY_MODEL(ped);

		if (PED::IS_PED_HUMAN(ped))
			Features::g_AddedPeds.push_back(ped);

		if (model)
		{
			const auto it = g_PedModelNameList.find(model);
			if (it != g_PedModelNameList.end())
				LOG_TO_MENU("Creating persistent character %s (0x%X) hash: 0x%X, ID: 0x%X\n", it->second.data(), model, hash, ret);
		}
#endif
	}
	
	Any ForceSpawnPersChar4Hook(scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["spawned_human"].get<bool>())
			return ForceSpawnPersChar4.GetOriginal<decltype(&ForceSpawnPersChar4Hook)>()(ctx);

		Hash p0 = ctx->GetArg<Hash>(0);
		Any result = ForceSpawnPersChar4.GetOriginal<decltype(&ForceSpawnPersChar4Hook)>()(ctx);
		Any ret = ctx->GetRet<Any>();

		//LOG_TO_MENU(__FUNCTION__"(0x%X) = %d\n", p0, ret);

		//Ped ped = PERSCHAR::_GET_PERSCHAR_PED_INDEX(persChar);
		//if (ped == 0 /*|| !ENTITY::DOES_ENTITY_EXIST(ped)*/)
		//{
		//	LOG_TO_CONSOLE(__FUNCTION__" returned an invalid ped index for perschar 0x%x!\n", persChar);
		//	return;
		//}
		//
		//Hash model = ENTITY::GET_ENTITY_MODEL(ped);
		//
		//LOG_TO_MENU(__FUNCTION__": Spawning persistent character %s (0x%X), ID: 0x%X\n", Features::GetPedModelName(model).c_str(), model, ped);
		//
		//if (PED::IS_PED_HUMAN(ped))
		//	Features::g_AddedPeds.push_back(ped);

		return result;
	}
#endif

	Object CreateObjectHook(scrNativeCallContext* ctx)
	{
		if (!ctx)
			return CreateObject.GetOriginal<decltype(&CreateObjectHook)>()(ctx);

		// Object CREATE_OBJECT(Hash modelHash, float x, float y, float z, BOOL isNetwork, BOOL bScriptHostObj, BOOL dynamic, BOOL p7, BOOL p8)

		Hash modelHash = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);
		BOOL isNetwork = ctx->GetArg<BOOL>(4);
		BOOL bScriptHostObj = ctx->GetArg<BOOL>(5);
		BOOL dynamic = ctx->GetArg<BOOL>(6);
		BOOL p7 = ctx->GetArg<BOOL>(7);
		BOOL p8 = ctx->GetArg<BOOL>(8);

		Object result = CreateObject.GetOriginal<decltype(&CreateObjectHook)>()(ctx);
		Object ret = ctx->GetRet<Object>();

		LOG_TO_MENU("OBJECT::CREATE_OBJECT(%u, %.2f, %.2f, %.2f, %s, %s, %s, %s, %s)\n\t-> %d\n", modelHash, pos.x, pos.y, pos.z,
			(isNetwork ? "TRUE" : "FALSE"), (bScriptHostObj ? "TRUE" : "FALSE"), (dynamic ? "TRUE" : "FALSE"),
			(p7 ? "TRUE" : "FALSE"), (p8 ? "TRUE" : "FALSE"), ret);

		Features::LogSpawnedEntity(ctx->GetRet<Object>());

		return result;
	}
	
	Object CreateObjectNoOffsetHook(scrNativeCallContext* ctx)
	{
		if (!ctx)
			return CreateObjectNoOffset.GetOriginal<decltype(&CreateObjectNoOffsetHook)>()(ctx);

		// Object CREATE_OBJECT_NO_OFFSET(Hash modelHash, float x, float y, float z, BOOL isNetwork, BOOL bScriptHostObj, BOOL dynamic, BOOL p7)

		Hash modelHash = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);
		BOOL isNetwork = ctx->GetArg<BOOL>(4);
		BOOL bScriptHostObj = ctx->GetArg<BOOL>(5);
		BOOL dynamic = ctx->GetArg<BOOL>(6);
		BOOL p7 = ctx->GetArg<BOOL>(7);

		Object result = CreateObjectNoOffset.GetOriginal<decltype(&CreateObjectNoOffsetHook)>()(ctx);
		Object ret = ctx->GetRet<Object>();

		LOG_TO_MENU("OBJECT::CREATE_OBJECT_NO_OFFSET(%u, %.2f, %.2f, %.2f, %s, %s, %s, %s)\n\t-> %d\n", modelHash, pos.x, pos.y, pos.z,
			(isNetwork ? "TRUE" : "FALSE"), (bScriptHostObj ? "TRUE" : "FALSE"), (dynamic ? "TRUE" : "FALSE"),
			(p7 ? "TRUE" : "FALSE"), ret);
		
		Features::LogSpawnedEntity(ctx->GetRet<Object>());

		return result;
	}
}
