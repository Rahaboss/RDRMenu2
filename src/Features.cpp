#include "pch.h"
#include "Features.h"
#include "Pointers.h"
#include "rage/natives.h"

void Features::ExecuteAsThread(rage::joaat_t ScriptHash, void (*Function)())
{
	for (uint16_t i = 0; i < Pointers::ThreadCollection->m_pCount; i++)
	{
		rage::GtaThread* Thread = Pointers::ThreadCollection->m_pData[i];
		if (!Thread || !Thread->m_ThreadID || Thread->m_ScriptHash != ScriptHash)
			continue;

		// Backup and change thread
		rage::scrThread* OriginalThread = *Pointers::ActiveThread;
		*Pointers::ActiveThread = Thread;

		// Execute
		Function();

		// Restore thread 
		*Pointers::ActiveThread = OriginalThread;
		return;
	}
}

void Features::OnSetup()
{
	std::cout << PLAYER::GET_PLAYER_NAME(PLAYER::PLAYER_ID()) << "\n";
}

void Features::OnTick()
{
}

void Features::OnExit()
{
}
