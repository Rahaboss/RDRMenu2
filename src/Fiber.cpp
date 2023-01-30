#include "pch.h"
#include "Fiber.h"
#include "Features.h"

Fiber::Fiber(void(*Function)()) :
	m_ScriptFiber(nullptr),
	m_MainFiber(nullptr),
	m_Function(Function)
{
	printf("Creating fiber 0x%llX\n", (uint64_t)m_Function);
	m_ScriptFiber = CreateFiber(0, [](void* FiberParam)
		{
			Fiber* ThisFiber = static_cast<Fiber*>(FiberParam);
			while (true)
			{
				TRY
				{
					ThisFiber->m_Function();
				}
				EXCEPT{ LOG_EXCEPTION(); }
				ThisFiber->YieldThread();
			}
		}, this);
}

void Fiber::Destroy()
{
	printf("Destroying fiber 0x%llX\n", (uint64_t)m_Function);
	if (m_ScriptFiber)
		DeleteFiber(m_ScriptFiber);
}

void Fiber::YieldThread()
{
	SwitchToFiber(m_MainFiber);
}

void Fiber::Tick()
{
	m_MainFiber = GetCurrentFiber();
	SwitchToFiber(m_ScriptFiber);
}

void ScriptThreadTick()
{
	static bool Initialized = false;
	if (!Initialized)
	{
		if (!IsThreadAFiber())
			ConvertThreadToFiber(NULL);
		Features::Setup();
		Initialized = true;
	}

	for (const auto& CurrentFiber : g_FiberCollection)
	{
		TRY
		{
			CurrentFiber->Tick();
		}
		EXCEPT
		{
			LOG_EXCEPTION();
			printf("Exception in Fiber Tick: 0x%llX.\n", reinterpret_cast<uint64_t>(CurrentFiber));
		}
	}
}
