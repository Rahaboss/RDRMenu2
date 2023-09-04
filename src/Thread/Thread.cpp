#include "Thread.h"
#include "rage/scrThread.h"
#include "Memory/Pointers.h"
#include "Fiber.h"

static rage::scrThread* GetActiveThread()
{
	if (Pointers::ActiveThread)
		return *Pointers::ActiveThread;
	return nullptr;
}

static void SetActiveThread(rage::scrThread* Thread)
{
	if (Pointers::ActiveThread)
		*Pointers::ActiveThread = Thread;
}

void Thread::ExecuteAsThread(rage::joaat_t ScriptHash, void (*Function)())
{
	for (uint16_t i = 0; i < Pointers::ThreadCollection->m_pCount; i++)
	{
		rage::GtaThread* Thread = Pointers::ThreadCollection->m_pData[i];
		if (!Thread || !Thread->m_ThreadID || Thread->m_ScriptHash != ScriptHash)
			continue;

		// Backup and change thread
		rage::scrThread* OriginalThread = GetActiveThread();
		SetActiveThread(Thread);

		// Execute
		Function();

		// Restore thread
		SetActiveThread(OriginalThread);
		
		return;
	}
}

void Thread::YieldThread()
{
	Fiber::GetCurrent()->YieldThread();
}
