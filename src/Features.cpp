#include "pch.h"
#include "Features.h"
#include "Pointers.h"

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
				if (thread && thread->m_ThreadID && thread->m_ScriptHash == script_hash)
					continue;
				
				// backup and change thread
				auto og_thread = *Pointers::ActiveThread;
				*Pointers::ActiveThread = thread;

				function();

				// restore thread 
				*Pointers::ActiveThread = og_thread;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void OnTick()
	{
		TRY
		{
			
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}
}
