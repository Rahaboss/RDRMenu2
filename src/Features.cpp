#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "rage/natives.h"

namespace Features
{
	void ExecuteAsThread(joaat_t script_hash, void(*function)())
	{
		TRY
		{
			if (!function || !Pointers::ThreadCollection || !Pointers::ActiveThread)
				return;

			for (uint16_t i = 0; i < Pointers::ThreadCollection->m_pCount; i++)
			{
				auto thread = Pointers::ThreadCollection->m_pData[i];
				if (!thread || !thread->m_ThreadID || thread->m_ScriptHash != script_hash)
					continue;
				
				// backup and change thread
				auto og_thread = *Pointers::ActiveThread;
				*Pointers::ActiveThread = thread;

				// execute
				function();

				// restore thread 
				*Pointers::ActiveThread = og_thread;
				return;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }

		std::cout << "Couldn't execute as thread!\n";
	}

	void OnTick()
	{
		TRY
		{
			if (GetAsyncKeyState(VK_PRIOR) & 1)
			{
				std::cout << "Hash of current script: " << SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME() << "\n";
				std::cout << "RAGE_JOAAT(\"main\"): " << RAGE_JOAAT("main") << "\n";
				std::cout << "SCRIPTS::GET_HASH_OF_THIS_SCRIPT_NAME handler: " << Pointers::GetNativeHandler(0xBC2C927F5C264960) << "\n";
				std::cout << "Player name: " << PLAYER::GET_PLAYER_NAME(0) << "\n";
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	int FpsTick()
	{
		int result = 0;

		static auto fc_begin = *Pointers::FrameCount;
		static auto time_begin = GetTickCount64();

		auto time_new = GetTickCount64();
		if (time_begin + 1000 < time_new)
		{
			auto fc_new = *Pointers::FrameCount;
			result = fc_new - fc_begin;

			fc_begin = *Pointers::FrameCount;
			time_begin = time_new;
		}

		return result;
	}
}
