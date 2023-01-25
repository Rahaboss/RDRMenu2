#include "pch.h"
#include "Thread.h"
#include "Pointers.h"
#include "JobQueue.h"
#include "Fiber.h"

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

	void YieldThread()
	{
		Fiber::GetCurrent()->YieldThread();
	}
}
