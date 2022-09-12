#pragma once

#include "pch.h"
#include "DetourHook.h"
#include "rage/scrThread.h"
#include "rage/scrNativeCallContext.h"

namespace Hooking
{
	void Create();
	void Destroy();
	void Enable();
	void Disable();

	inline DetourHook RunScriptThreads;
	bool RunScriptThreadsHook(pgPtrCollection* this_, uint32_t ops);
	//inline DetourHook RunScriptThreads2;
	//bool RunScriptThreadsHook2(uint32_t ops);
	inline DetourHook ShootBullet;
	void ShootBulletHook(scrNativeCallContext* ctx);
	inline DetourHook IsEntityInArea;
	BOOL IsEntityInAreaHook(scrNativeCallContext* ctx);
}
