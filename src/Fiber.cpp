#include "pch.h"
#include "Fiber.h"
#include "Features.h"

Fiber::Fiber(void(*func)()) :
	m_script_fiber(nullptr),
	m_main_fiber(nullptr),
	m_func(func)
{
	std::cout << "Creating fiber " << LOG_HEX(m_func) << ".\n";
	m_script_fiber = CreateFiber(0, [](void* param)
		{
			auto this_fiber = static_cast<Fiber*>(param);
			while (true)
			{
				TRY{ this_fiber->m_func(); }
				EXCEPT{ LOG_EXCEPTION(); }
				this_fiber->YieldThread();
			}
		}, this);
}

void Fiber::Destroy()
{
	std::cout << "Destroying fiber " << LOG_HEX(m_func) << ".\n";
	if (m_script_fiber)
		DeleteFiber(m_script_fiber);
}

void Fiber::YieldThread()
{
	SwitchToFiber(m_main_fiber);
}

void Fiber::Tick()
{
	m_main_fiber = GetCurrentFiber();
	SwitchToFiber(m_script_fiber);
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
		EXCEPT{ LOG_EXCEPTION(); std::cout << "Exception in Fiber Tick: " << fiber << '\n'; }
	}
}
