#pragma once

#include "pch.h"
#include "JobQueue.h"

class Fiber
{
public:
	Fiber(std::function<void()> Function, LPFIBER_START_ROUTINE FiberFunction);
	Fiber(const Fiber&) = delete;
	Fiber(Fiber&&) = delete;
	~Fiber();
	void Destroy();
	void YieldThread();
	void Tick();
	static Fiber* GetCurrent() { return reinterpret_cast<Fiber*>(GetFiberData()); }

	void* m_ScriptFiber;
	void* m_MainFiber;
	std::function<void()> m_Function;
	void* m_ExtraData = nullptr;

	static void ScriptThreadTick();
	static void MainFiberFunction(void* FiberParam);
	static void JobQueueFiberFunction(void* FiberParam);
};

inline std::vector<Fiber*> g_FiberCollection;
