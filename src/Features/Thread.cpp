#include "pch.h"
#include "Thread.h"
#include "Pointers.h"
#include "JobQueue.h"
#include "Fiber.h"
#include "Features.h"

namespace Features
{
	void ExecuteAsThread(joaat_t ScriptHash, void(*Function)())
	{
		TRY
		{
			assert(Function && Pointers::ThreadCollection && Pointers::ActiveThread);

			for (uint16_t i = 0; i < Pointers::ThreadCollection->m_pCount; i++)
			{
				GtaThread* Thread = Pointers::ThreadCollection->m_pData[i];
				if (!Thread || !Thread->m_ThreadID || Thread->m_ScriptHash != ScriptHash)
					continue;

				// Backup and change thread
				scrThread* OriginalThread = *Pointers::ActiveThread;
				*Pointers::ActiveThread = Thread;

				// Execute
				Function();

				// Restore thread 
				*Pointers::ActiveThread = OriginalThread;
				return;
			}
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	void RunJobQueue()
	{
		g_JobQueue.Run();
	}

	void YieldThread(uint32_t DurationMS)
	{
		const uint64_t EndTime = GetTickCount64() + DurationMS;
		do
		{
			Fiber::GetCurrent()->YieldThread();
		} while (GetTickCount64() < EndTime);
	}
	
	void RunScriptCleanupAndExit()
	{
		QUEUE_JOB(=)
		{
			Cleanup();
			g_Running = false;
		}
		END_JOB()
		std::this_thread::sleep_for(100ms);
		g_Running = false;
	}
}
