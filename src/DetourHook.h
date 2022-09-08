#pragma once

#include "pch.h"

class DetourHook
{
public:
	void Create(void* target, void* detour);
	void Destroy();

	template<typename T>
	T GetOriginal() { return reinterpret_cast<T>(m_original); }

private:
	void* m_target{};
	void* m_original{};
};
