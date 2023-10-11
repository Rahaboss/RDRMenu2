#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Renderer/Renderer.h"
#include "Script/Features.h"
#include "Thread/Fiber.h"
#include "Thread/Thread.h"
#include "Config/Settings.h"
#include "Rage/natives.h"
#include "Script/PlayerInfo.h"
#include "Util/String.h"
#include "Config/Lists.h"
#include "Script/Entity.h"

void Hooking::Create()
{
	LOG_TEXT("Creating hooks.");

	const MH_STATUS Status = MH_Initialize();
	assert(Status == MH_OK);

	RunScriptThreads.Create(Pointers::RunScriptThreads, RunScriptThreadsHook);
	IsDLCPresent.Create(NativeInvoker::GetHandler(0x2763DC12BBE2BB6F), IsDLCPresentHook);
	DecreaseAmmo.Create(Pointers::DecreaseAmmo, DecreaseAmmoHook);
	ShootBullet.Create(NativeInvoker::GetHandler(0x867654CBC7606F2C), ShootBulletHook);
	IsEntityInArea.Create(NativeInvoker::GetHandler(0xD3151E53134595E5), IsEntityInAreaHook);
#ifndef _DIST
	CreateAnimScene.Create(NativeInvoker::GetHandler(0x1FCA98E33C1437B3), CreateAnimSceneHook);
	SetAnimSceneEntity.Create(NativeInvoker::GetHandler(0x8B720AD451CA2AB3), SetAnimSceneEntityHook);
	StartAnimScene.Create(NativeInvoker::GetHandler(0xF4D94AF761768700), StartAnimSceneHook);
#endif // !_DIST
}

void Hooking::Destroy()
{
	LOG_TEXT("Destroying hooks.");
	
#ifndef _DIST
	StartAnimScene.Destroy();
	SetAnimSceneEntity.Destroy();
	CreateAnimScene.Destroy();
#endif // !_DIST
	IsEntityInArea.Destroy();
	ShootBullet.Destroy();
	DecreaseAmmo.Destroy();
	IsDLCPresent.Destroy();
	RunScriptThreads.Destroy();

	const MH_STATUS Status = MH_Uninitialize();
	assert(Status == MH_OK);
}

void Hooking::Enable()
{
	LOG_TEXT("Enabling hooks.");

	const MH_STATUS Status = MH_EnableHook(MH_ALL_HOOKS);
	assert(Status == MH_OK);
}

void Hooking::Disable()
{
	LOG_TEXT("Disabling hooks.");

	const MH_STATUS Status = MH_DisableHook(MH_ALL_HOOKS);
	assert(Status == MH_OK);
}

HRESULT STDMETHODCALLTYPE Hooking::SwapChainPresentHook(IDXGISwapChain3* SwapChain, UINT SyncInterval, UINT Flags)
{
	if (g_Running)
		RendererD3D12::Present(SwapChain);

	return Hooking::SwapChain.GetOriginal<decltype(&SwapChainPresentHook)>(SwapChainPresentIndex)(SwapChain, SyncInterval, Flags);
}

bool Hooking::RunScriptThreadsHook(rage::pgPtrCollection* this_, uint32_t ops)
{
	const bool Result = RunScriptThreads.GetOriginal<decltype(&RunScriptThreadsHook)>()(this_, ops);

	if (g_Running)
		Thread::ExecuteAsThread(RAGE_JOAAT("main"), &Fiber::ScriptThreadTick);

	return Result;
}

void Hooking::IsDLCPresentHook(rage::scrNativeCallContext* ctx)
{
	Hash dlcHash = ctx->GetArg<Hash>(0);

	IsDLCPresent.GetOriginal<decltype(&IsDLCPresentHook)>()(ctx);

	if (!g_Settings["enable_dlcs"].get<bool>())
		return;

	switch (dlcHash)
	{
	case RAGE_JOAAT("DLC_PREORDERCONTENT"):
	case RAGE_JOAAT("DLC_PHYSPREORDERCONTENT"):
	case RAGE_JOAAT("DLC_SPECIALEDITION"):
	case RAGE_JOAAT("DLC_ULTIMATEEDITION"):
	case RAGE_JOAAT("DLC_TREASUREMAP"):
		ctx->SetRet<BOOL>(TRUE);
		return;
	}

	LOG_TEXT("%s: Unknown DLC hash %u / 0x%X.", __FUNCTION__, dlcHash, dlcHash);
}

void Hooking::DecreaseAmmoHook(void* a1, rage::CPed* a2, uint64_t a3, uint32_t a4)
{
	if (a2 == g_LocalPlayer.m_Ped && g_Settings["weapon"]["infinite_ammo"].get<bool>())
		return;

	DecreaseAmmo.GetOriginal<decltype(&DecreaseAmmoHook)>()(a1, a2, a3, a4);
}

void Hooking::ShootBulletHook(rage::scrNativeCallContext* ctx)
{
	if (g_Settings["disable_invisible_snipers"].get<bool>() && ctx->GetArg<Hash>(8) == RAGE_JOAAT("WEAPON_SNIPERRIFLE_CARCANO"))
		return;

	ShootBullet.GetOriginal<decltype(&ShootBulletHook)>()(ctx);
}

void Hooking::IsEntityInAreaHook(rage::scrNativeCallContext* ctx)
{
	if (g_Settings["disable_invisible_snipers"].get<bool>() && ctx->GetArg<Entity>(0) == g_LocalPlayer.m_Entity &&
		ctx->GetArg<uint32_t>(1) == 0x44BBD654)
	{
		ctx->SetRet<BOOL>(FALSE); // Spoof return value
		return;
	}

	IsEntityInArea.GetOriginal<decltype(&IsEntityInAreaHook)>()(ctx);
}

#ifndef _DIST
static std::unordered_map<AnimScene, std::string> s_AnimScenes;
void Hooking::CreateAnimSceneHook(rage::scrNativeCallContext* ctx)
{
	const char* animDict = ctx->GetArg<const char*>(0);
	const char* playbackListName = ctx->GetArg<const char*>(2);

	CreateAnimScene.GetOriginal<decltype(&CreateAnimSceneHook)>()(ctx);

	AnimScene animScene = ctx->GetRet<AnimScene>();

	if (g_Settings["log_animscene"].get<bool>())
	{
		if (Util::IsStringValid(playbackListName))
			LOG_TEXT("Created AnimScene \"%s\" (\"%s\"), ID: %u.", animDict, playbackListName, animScene);
		else
			LOG_TEXT("Created AnimScene \"%s\", ID: %u.", animDict, animScene);
	}

	s_AnimScenes[animScene] = std::string{ animDict };
}

void Hooking::SetAnimSceneEntityHook(rage::scrNativeCallContext* ctx)
{
	AnimScene animScene = ctx->GetArg<AnimScene>(0);
	const char* entityName = ctx->GetArg<const char*>(1);
	Entity entity = ctx->GetArg<Entity>(2);

	if (ANIMSCENE::_GET_ANIM_SCENE_PED(animScene, entityName, false) != entity)
	{
		Hash model = Script::GetEntityModel(entity);
		std::string ModelName = Lists::GetHashName(model);

		if (ModelName.empty())
			ModelName = std::to_string(model);
		else
		{
			Util::StringToLower(ModelName);
			ModelName = '\"' + ModelName + '\"';
		}

		if (g_Settings["log_animscene"].get<bool>())
		{
			const auto it = s_AnimScenes.find(animScene);
			if (it != s_AnimScenes.end())
				LOG_TEXT("Added entity %s (\"%s\") to AnimScene \"%s\", ID: %u.", ModelName.c_str(), entityName, it->second.c_str(), animScene);
			else
				LOG_TEXT("Added entity %s (\"%s\") to AnimScene ID: %u.", ModelName.c_str(), entityName, animScene);
		}
	}
	
	SetAnimSceneEntity.GetOriginal<decltype(&SetAnimSceneEntityHook)>()(ctx);
}

void Hooking::StartAnimSceneHook(rage::scrNativeCallContext* ctx)
{
	AnimScene animScene = ctx->GetArg<AnimScene>(0);
	Vector3 position, rotation;
	ANIMSCENE::GET_ANIM_SCENE_ORIGIN(animScene, &position, &rotation, 2);

	if (g_Settings["log_animscene"].get<bool>())
	{
		const auto it = s_AnimScenes.find(animScene);
		if (it != s_AnimScenes.end())
			LOG_TEXT("Starting AnimScene \"%s\", ID: %u at: %.2f, %.2f, %.2f.", it->second.c_str(), animScene, position.x, position.y, position.z);
		else
			LOG_TEXT("Starting AnimScene ID: %u at: %.2f, %.2f, %.2f.", animScene, position.x, position.y, position.z);
	}

	StartAnimScene.GetOriginal<decltype(&StartAnimSceneHook)>()(ctx);
}
#endif // !_DIST
