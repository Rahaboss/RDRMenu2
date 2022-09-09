#include "pch.h"
#include "Hooking.h"
#include "Pointers.h"
#include "Console.h"
#include "Features.h"

namespace Hooking
{
	static bool Enabled = false;

	void Create()
	{
		assert(MH_Initialize() == MH_OK);
		RunScriptThreads.Create(Pointers::RunScriptThreads, RunScriptThreadsHook);
		//RunScriptThreads2.Create(Pointers::RunScriptThreads2, RunScriptThreadsHook2);
	}

	void Destroy()
	{
		//RunScriptThreads2.Destroy();
		RunScriptThreads.Destroy();
		assert(MH_Uninitialize() == MH_OK);
	}

	void Enable()
	{
		if (Enabled)
			return;
		assert(MH_EnableHook(MH_ALL_HOOKS) == MH_OK);
		Enabled = true;
	}

	void Disable()
	{
		if (!Enabled)
			return;
		assert(MH_DisableHook(MH_ALL_HOOKS) == MH_OK);
		Enabled = false;
	}

	bool RunScriptThreadsHook(pgPtrCollection* this_, uint32_t ops)
	{
		TRY
		{
			if (g_running)
			{
				constexpr joaat_t main_hash = RAGE_JOAAT("main");
				Features::ExecuteAsThread(main_hash, Features::OnTick);
				
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
	//			std::cout << "RunScriptThreadsHook2(" << ops << ")\n";
	//		
	//			return RunScriptThreads2.GetOriginal<decltype(&RunScriptThreadsHook2)>()(ops);
	//		}
	//	}
	//	EXCEPT{ LOG_EXCEPTION(); }
	//
	//	return false;
	//}
}
