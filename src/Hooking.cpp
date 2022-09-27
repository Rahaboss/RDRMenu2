#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Console.h"
#include "Features.h"
#include "Fiber.h"
#include "PlayerInfo.h"

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
	}

	void Destroy()
	{
		std::cout << "Destroying hooks.\n";

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
	//			std::cout << __FUNCTION__"(" << ops << ")\n";
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
			if (Features::EnableNoSnipers && *(Hash*)(&ctx->m_Args[8]) == WEAPON_SNIPERRIFLE_CARCANO)
				return;
		
			ShootBullet.GetOriginal<decltype(&ShootBulletHook)>()(ctx);
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	BOOL IsEntityInAreaHook(scrNativeCallContext* ctx)
	{
		TRY
		{
			if (Features::EnableNoSnipers && *(Entity*)(&ctx->m_Args[0]) == g_LocalPlayer.m_Entity &&
				*(uint32_t*)(&ctx->m_Args[1]) == 0x44BBD654) // 1502.69775391f
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
			if (Features::EnableNoReload && a2 == Pointers::GetPlayerPed(g_LocalPlayer.m_Index))
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
			if (!Features::EnableHumanSpawnLogging && !Features::EnablePedSpawnLogging)
			{
				result = Hooking::CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);
			}
			else if (ctx)
			{
				Hash model = *(Hash*)(&(ctx->m_Args[0]));
				Vector3 pos = *(Vector3*)(&(ctx->m_Args[1]));

				result = Hooking::CreatePed.GetOriginal<decltype(&CreatePedHook)>()(ctx);
				Ped id = *(Ped*)(ctx->m_ReturnValue);

				if (PED::IS_PED_HUMAN(id))
				{
					if (Features::EnableHumanSpawnLogging)
					{
						std::cout << "Creating human " << Features::GetPedModelName(model) << " (" << LOG_HEX(model)
							<< ") ID: " << LOG_HEX(id) << " at: " << pos << ".\n";
					}
				}
				else if (Features::EnablePedSpawnLogging)
				{
					std::cout << "Creating ped " << Features::GetPedModelName(model) << " (" << LOG_HEX(model)
						<< ") ID: " << LOG_HEX(id) << " at: " << pos << ".\n";
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
			if (Features::EnableVehicleSpawnLogging && ctx)
			{
				Hash model = *(Hash*)(&(ctx->m_Args[0]));
				Vector3 pos = *(Vector3*)(&(ctx->m_Args[1]));

				result = Hooking::CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);
				Vehicle id = *(Vehicle*)(ctx->m_ReturnValue);

				std::cout << "Creating vehicle " << Features::GetVehicleModelName(model) << " (" << LOG_HEX(model)
					<< ") ID: " << LOG_HEX(id) << " at: " << pos << ".\n";
			}
			else
			{
				result = Hooking::CreateVehicle.GetOriginal<decltype(&CreateVehicleHook)>()(ctx);
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		return result;
	}
}
