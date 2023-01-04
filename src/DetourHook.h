#pragma once

#include "pch.h"

class DetourHook
{
public:
	void Create(void* Target, void* Detour);
	void Destroy();

	template<typename T>
	T GetOriginal() { return reinterpret_cast<T>(m_Original); }

private:
	void* m_Target{};
	void* m_Original{};
};
