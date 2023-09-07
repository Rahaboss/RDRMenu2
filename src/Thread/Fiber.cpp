#include "pch.h"
#include "Fiber.h"
#include "Script/Features.h"
#include "Util/Timer.h"

Fiber::Fiber(void (*Function)()) :
	m_ScriptFiber(),
	m_MainFiber(),
	m_Function(Function)
{
	LOG_TEXT("Creating fiber 0x%llX.\n", (uintptr_t)m_Function);

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
	LOG_TEXT("Destroying fiber 0x%llX.\n", (uintptr_t)m_Function);

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

static bool s_Initialized = false;
void Fiber::ScriptThreadTick()
{
	Timer t;

	if (!s_Initialized)
	{
		if (!IsThreadAFiber())
			ConvertThreadToFiber(NULL);

		Features::OnSetup();

		s_Initialized = true;
	}

	for (const auto& CurrentFiber : g_FiberCollection)
	{
		TRY
		{
			CurrentFiber->Tick();
		}
		EXCEPT{ LOG_EXCEPTION(); }
	}

	Timer::s_ScriptThreadTime = t.GetMillis();
}
