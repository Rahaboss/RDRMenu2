#include "pch.h"
#include "Fiber.h"
#include "Features.h"

Fiber::Fiber(void(*Function)()) :
	m_ScriptFiber(nullptr),
	m_MainFiber(nullptr),
	m_Function(Function)
{
	std::cout << "Creating fiber " << LOG_HEX(m_Function) << ".\n";

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
	std::cout << "Destroying fiber " << LOG_HEX(m_Function) << ".\n";

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
void ScriptThreadTick()
{
	if (!s_Initialized)
	{
		if (!IsThreadAFiber())
			ConvertThreadToFiber(NULL);

		Features::Setup();

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
}
