#include "pch.h"
#include "Fiber.h"
#include "Features.h"

Fiber::Fiber(void(*func)()) :
	m_ScriptFiber(nullptr),
	m_MainFiber(nullptr),
	m_Func(func)
{
	printf("Creating fiber 0x%llX\n", (uint64_t)m_Func);
	m_ScriptFiber = CreateFiber(0, [](void* param)
		{
			auto ThisFiber = static_cast<Fiber*>(param);
			while (true)
			{
				TRY{ ThisFiber->m_Func(); }
				EXCEPT{ LOG_EXCEPTION(); }
				ThisFiber->YieldThread();
			}
		}, this);
}

void Fiber::Destroy()
{
	printf("Destroying fiber 0x%llX\n", (uint64_t)m_Func);
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

static bool init = false;
void ScriptThreadTick()
{
	if (!init)
	{
		if (!IsThreadAFiber())
			ConvertThreadToFiber(NULL);
		Features::Setup();
		init = true;
	}

	for (const auto& fiber : g_FiberCollection)
	{
		TRY
		{
			fiber->Tick();
		}
		EXCEPT{ LOG_EXCEPTION(); printf("Exception in Fiber Tick: 0x%llX.\n", (uint64_t)fiber); }
	}
}
