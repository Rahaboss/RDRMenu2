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
		CreateObject.Create(NativeContext::GetHandler(0x509D5878EB39E842), CreateObjectHook);
		CreateObjectNoOffset.Create(NativeContext::GetHandler(0x9A294B2138ABB884), CreateObjectNoOffsetHook);
		GetLabelText2.Create(NativeContext::GetHandler(0x3429670F9B9EF2D3), GetLabelText2Hook);
		GetCharFromAudioConvFilename.Create(NativeContext::GetHandler(0x9D7E12EC6A1EE4E5), GetCharFromAudioConvFilenameHook);
	}

	void Destroy()
	{
		LOG_TO_CONSOLE("Destroying hooks.\n");

		GetCharFromAudioConvFilename.Destroy();
		GetLabelText2.Destroy();
		CreateObjectNoOffset.Destroy();
		CreateObject.Destroy();
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

		assert(HookCount == 0);
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

	bool RunScriptThreadsHook(rage::pgPtrCollection* this_, uint32_t ops)
	{
		TRY
		{
			bool Result = RunScriptThreads.GetOriginal<decltype(&RunScriptThreadsHook)>()(this_, ops);
			
			if (g_Running)
				Features::ExecuteAsThread(RAGE_JOAAT("main"), ScriptThreadTick);
			
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
				Features::ExecuteAsThread(RAGE_JOAAT("main"), ScriptThreadTick);

			return Result;
		}
		EXCEPT{ LOG_EXCEPTION(); }
	
		return false;
	}

	void ShootBulletHook(rage::scrNativeCallContext* ctx)
	{
		if (ctx && g_Settings["no_snipers"].get<bool>() &&
			ctx->GetArg<Hash>(8) == WEAPON_SNIPERRIFLE_CARCANO)
			return;
			
		ShootBullet.GetOriginal<decltype(&ShootBulletHook)>()(ctx);
	}

	BOOL IsEntityInAreaHook(rage::scrNativeCallContext* ctx)
	{
		TRY
		{
			if (g_Settings["no_snipers"].get<bool>() && ctx &&
				ctx->GetArg<Entity>(0) == g_LocalPlayer.m_Entity &&
				ctx->GetArg<uint32_t>(1) == 0x44BBD654) // 1502.69775391f
			{
				ctx->SetRet<BOOL>(FALSE); // Spoof return value
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

	void DecreaseAmmoHook(uint64_t a1, rage::CPed* a2, uint64_t a3, uint32_t a4)
	{
		TRY
		{
			if (a2 == g_LocalPlayer.m_Ped && g_Settings["infinite_ammo"].get<bool>())
				return;

			DecreaseAmmo.GetOriginal<decltype(&DecreaseAmmoHook)>()(a1, a2, a3, a4);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	Ped CreatePedHook(rage::scrNativeCallContext* ctx)
	{
		if (!ctx)
			return CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);

		Ped result = CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);

		Features::LogSpawnedEntity(ctx->GetRet<Ped>());

		return result;
	}
	
	Vehicle CreateVehicleHook(rage::scrNativeCallContext* ctx)
	{
		if (!ctx)
			return CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);

		Vehicle result = CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);

		Features::LogSpawnedEntity(ctx->GetRet<Vehicle>());

		return result;
	}
	
	BOOL InventoryAddItemHook(rage::scrNativeCallContext* ctx)
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
			case RAGE_JOAAT("CONSUMABLE_BIG_GAME_MEAT_COOKED"):
			case RAGE_JOAAT("CONSUMABLE_BIG_GAME_MEAT_OREGANO_COOKED"):
			case RAGE_JOAAT("CONSUMABLE_BIG_GAME_MEAT_THYME_COOKED"):
			case RAGE_JOAAT("CONSUMABLE_BIG_GAME_MEAT_WILD_MINT_COOKED"):
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
	
	BOOL GetGUIDFromItemIDHook(rage::scrNativeCallContext* ctx)
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

			switch (inventoryId)
			{
			case RAGE_JOAAT("CONSUMABLE_BIG_GAME_MEAT_COOKED"):
			case RAGE_JOAAT("CONSUMABLE_BIG_GAME_MEAT_OREGANO_COOKED"):
			case RAGE_JOAAT("CONSUMABLE_BIG_GAME_MEAT_THYME_COOKED"):
			case RAGE_JOAAT("CONSUMABLE_BIG_GAME_MEAT_WILD_MINT_COOKED"):
			case RAGE_JOAAT("CLOTHING_SP_CIVIL_WAR_HAT_000_1"):
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
				Renderer::Present();

			Result = Hooking::SwapChain.GetOriginal<decltype(&SwapChainPresentHook)>(SwapChainPresentIndex)(SwapChain, SyncInterval, Flags);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return Result;
	}

	HRESULT STDMETHODCALLTYPE SwapChainResizeBuffersHook(IDXGISwapChain* SwapChain, UINT BufferCount, UINT Width, UINT Height, DXGI_FORMAT NewFormat, UINT SwapChainFlags)
	{
		LOG_TO_CONSOLE(__FUNCTION__"(0x%llX, %u, %u, %u, %d, %u)\n", (uintptr_t)SwapChain, BufferCount, Width, Height, NewFormat, SwapChainFlags);

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

	PersChar CreatePersCharHook(rage::scrNativeCallContext* ctx)
	{
		if (!ctx || !g_Settings["logging"]["spawned_human"].get<bool>())
			return CreatePersChar.GetOriginal<decltype(&CreatePersCharHook)>()(ctx);

		Hash hash = ctx->GetArg<Hash>(0);
		PersChar result = CreatePersChar.GetOriginal<decltype(&CreatePersCharHook)>()(ctx);
		PersChar ret = ctx->GetRet<PersChar>();
		Hash model = PERSCHAR::_GET_PERSCHAR_MODEL_NAME(hash);

		if (!model)
			return result;
		
		const auto it = g_PedModelNameList.find(model);
		if (it == g_PedModelNameList.end())
			return result;
		
		LOG_TO_MENU("Creating persistent character %s (0x%X) hash: 0x%X, ID: 0x%X\n", it->second.data(), model, hash, ret);

		return result;
	}

	AnimScene CreateAnimSceneHook(rage::scrNativeCallContext* ctx)
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

	BOOL DecorSetBoolHook(rage::scrNativeCallContext* ctx)
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
				LOG_TO_MENU("DECOR_SET_BOOL(g_LocalPlayer.m_Entity, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else if (entity == g_LocalPlayer.m_Mount || entity == g_LocalPlayer.m_LastMount)
				LOG_TO_MENU("DECOR_SET_BOOL(g_LocalPlayer.m_Mount, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else if (entity == g_LocalPlayer.m_Vehicle)
				LOG_TO_MENU("DECOR_SET_BOOL(g_LocalPlayer.m_Vehicle, \"%s\", %s)\n", propertyName, (value ? "true" : "false"));
			else
				LOG_TO_MENU("DECOR_SET_BOOL(%u, \"%s\", %s)\n", entity, propertyName, (value ? "true" : "false"));
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}

	BOOL DecorSetIntHook(rage::scrNativeCallContext* ctx)
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
				LOG_TO_MENU("DECOR_SET_INT(g_LocalPlayer.m_Entity, \"%s\", %d)\n", propertyName, value);
			else if (entity == g_LocalPlayer.m_Mount || entity == g_LocalPlayer.m_LastMount)
				LOG_TO_MENU("DECOR_SET_INT(g_LocalPlayer.m_Mount, \"%s\", %d)\n", propertyName, value);
			else if (entity == g_LocalPlayer.m_Vehicle)
				LOG_TO_MENU("DECOR_SET_INT(g_LocalPlayer.m_Vehicle, \"%s\", %d)\n", propertyName, value);
			else
				LOG_TO_MENU("DECOR_SET_INT(%u, \"%s\", %d)\n", entity, propertyName, value);
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}

	void SetAnimSceneEntityHook(rage::scrNativeCallContext* ctx)
	{
		if (g_Settings["logging"]["added_cutscene_entity"].get<bool>())
		{
			AnimScene animScene = ctx->GetArg<AnimScene>(0);
			const char* entityName = ctx->GetArg<const char*>(1);
			Entity entity = ctx->GetArg<Entity>(2);
			int flags = ctx->GetArg<int>(3);
			Hash model = ENTITY::GET_ENTITY_MODEL(entity);
			
			std::string modelStr = Features::GetModelName(model);
			if (modelStr.empty())
			{
				if (rage::joaat(entityName) == model)
					modelStr = entityName;
				else
					modelStr = std::to_string(model);
			}

			LOG_TO_MENU("Adding animscene entity %s (model: %s) entity: 0x%X to animscene %d\n", entityName, modelStr.c_str(), entity, animScene);
		}

		SetAnimSceneEntity.GetOriginal<decltype(&SetAnimSceneEntityHook)>()(ctx);
	}
	
	BOOL IsDlcPresentHook(rage::scrNativeCallContext* ctx)
	{
		Hash dlcHash = ctx->GetArg<Hash>(0);
		BOOL result = IsDlcPresent.GetOriginal<decltype(&IsDlcPresentHook)>()(ctx);
		BOOL ret = ctx->GetRet<BOOL>();

		constexpr Hash DLCList[]{
			RAGE_JOAAT("DLC_PREORDERCONTENT"),
			RAGE_JOAAT("DLC_PHYSPREORDERCONTENT"),
			RAGE_JOAAT("DLC_SPECIALEDITION"),
			RAGE_JOAAT("DLC_ULTIMATEEDITION"),
			RAGE_JOAAT("DLC_TREASUREMAP"),
		};

		for (int i = 0; i < ARRAYSIZE(DLCList); i++)
		{
			if (dlcHash == DLCList[i])
			{
				ctx->SetRet<BOOL>(TRUE);
				result = TRUE;
				return result;
			}
		}

		LOG_TO_MENU("IS_DLC_PRESENT(%u) = %s\n", dlcHash, (ret ? "true" : "false"));
		
		return result;
	}
	
	Object CreateObjectHook(rage::scrNativeCallContext* ctx)
	{
		if (!ctx)
			return CreateObject.GetOriginal<decltype(&CreateObjectHook)>()(ctx);

		Hash modelHash = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);
		BOOL isNetwork = ctx->GetArg<BOOL>(4);
		BOOL bScriptHostObj = ctx->GetArg<BOOL>(5);
		BOOL dynamic = ctx->GetArg<BOOL>(6);
		BOOL p7 = ctx->GetArg<BOOL>(7);
		BOOL p8 = ctx->GetArg<BOOL>(8);

		Object result = CreateObject.GetOriginal<decltype(&CreateObjectHook)>()(ctx);
		Object ret = ctx->GetRet<Object>();

		Features::LogSpawnedEntity(ret);

		return result;
	}
	
	Object CreateObjectNoOffsetHook(rage::scrNativeCallContext* ctx)
	{
		if (!ctx)
			return CreateObjectNoOffset.GetOriginal<decltype(&CreateObjectNoOffsetHook)>()(ctx);

		Hash modelHash = ctx->GetArg<Hash>(0);
		Vector3 pos = ctx->GetArg<Vector3>(1);
		BOOL isNetwork = ctx->GetArg<BOOL>(4);
		BOOL bScriptHostObj = ctx->GetArg<BOOL>(5);
		BOOL dynamic = ctx->GetArg<BOOL>(6);
		BOOL p7 = ctx->GetArg<BOOL>(7);

		Object result = CreateObjectNoOffset.GetOriginal<decltype(&CreateObjectNoOffsetHook)>()(ctx);
		Object ret = ctx->GetRet<Object>();

		Features::LogSpawnedEntity(ret);

		return result;
	}
	
	const char* GetLabelText2Hook(rage::scrNativeCallContext* ctx)
	{
		if (!ctx)
			return GetLabelText2.GetOriginal<decltype(&GetLabelText2Hook)>()(ctx);

		const char* label = ctx->GetArg<const char*>(0);
		const char* result = GetLabelText2.GetOriginal<decltype(&GetLabelText2Hook)>()(ctx);
		const char* ret = ctx->GetRet<const char*>();

		//ret = result = ctx->GetRet<const char*>() = "RDRMenu2";
		LOG_TO_MENU("_GET_LABEL_TEXT_2(\"%s\") = \"%s\"\n", label, (ret ? ret : ""));
		
		return result;
	}
	
	const char* GetCharFromAudioConvFilenameHook(rage::scrNativeCallContext* ctx)
	{
		if (!ctx)
			return GetCharFromAudioConvFilename.GetOriginal<decltype(&GetCharFromAudioConvFilenameHook)>()(ctx);

		const char* text = ctx->GetArg<const char*>(0);
		int position = ctx->GetArg<int>(1);
		int length = ctx->GetArg<int>(2);
		const char* result = GetCharFromAudioConvFilename.GetOriginal<decltype(&GetCharFromAudioConvFilenameHook)>()(ctx);
		const char* ret = ctx->GetRet<const char*>();

		LOG_TO_MENU("_GET_TEXT_SUBSTRING(\"%s\", %d, %d) = \"%s\"\n", text, position, length, (ret ? ret : ""));

		return result;
	}
}
