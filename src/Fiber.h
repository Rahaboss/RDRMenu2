#pragma once

#include "pch.h"

class Fiber
{
public:
	Fiber(void (*Function)());
	void Destroy();
	void YieldThread();
	void Tick();
	static Fiber* GetCurrent() { return reinterpret_cast<Fiber*>(GetFiberData()); }

	void* m_ScriptFiber;
	void* m_MainFiber;
	void (*m_Function)();
};

inline std::vector<Fiber*> g_FiberCollection;

void ScriptThreadTick();
