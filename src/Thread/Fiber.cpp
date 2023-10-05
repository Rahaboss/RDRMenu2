#include "pch.h"
#include "Fiber.h"
#include "Script/Features.h"
#include "Util/Timer.h"

Fiber::Fiber(std::function<void()> Function, LPFIBER_START_ROUTINE FiberFunction) :
	m_ScriptFiber(),
	m_MainFiber(),
	m_Function(Function)
{
	m_ScriptFiber = CreateFiber(0, FiberFunction, this);
}

Fiber::~Fiber()
{
	Destroy();
}

void Fiber::Destroy()
{
	if (m_ScriptFiber)
	{
		DeleteFiber(m_ScriptFiber);
		m_ScriptFiber = 0;
	}
}

void Fiber::YieldThread()
{
	SwitchToFiber(m_MainFiber);
}

void Fiber::Tick()
{
	m_MainFiber = (IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(NULL));
	SwitchToFiber(m_ScriptFiber);
}

static bool s_Initialized = false;
void Fiber::ScriptThreadTick()
{
	Timer t;

	if (!s_Initialized)
	{
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

	TRY
	{
		JobQueue::Run();
	}
	EXCEPT{ LOG_EXCEPTION(); }

	Timer::s_ScriptThreadTime = t.GetMillis();
}

void Fiber::MainFiberFunction(void* FiberParam)
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
}

void Fiber::JobQueueFiberFunction(void* FiberParam)
{
	Fiber* ThisFiber = static_cast<Fiber*>(FiberParam);
	TRY
	{
		ThisFiber->m_Function();
	}
	EXCEPT{ LOG_EXCEPTION(); }
	*(bool*)ThisFiber->m_ExtraData = true;
	ThisFiber->YieldThread();
}
