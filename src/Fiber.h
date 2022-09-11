#pragma once

#include "pch.h"

class Fiber
{
public:
	Fiber(void (*func)());
	void Destroy();
	void YieldThread();
	void Tick();
	static Fiber* GetCurrent() { return static_cast<Fiber*>(GetFiberData()); }

	void* m_script_fiber;
	void* m_main_fiber;
	void (*m_func)();
};

inline std::vector<Fiber*> g_FiberCollection;

void ScriptThreadTick();
