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

	void* m_ScriptFiber;
	void* m_MainFiber;
	void (*m_Func)();
};

inline std::vector<Fiber*> g_FiberCollection;

void ScriptThreadTick();
