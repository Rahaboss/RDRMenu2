#pragma once

#include "pch.h"

class DetourHook
{
public:
	void Create(void* Target, void* Detour);
	void Destroy();

	template<typename T>
	inline std::enable_if_t<std::is_pointer_v<T>, T> GetOriginal()
	{
		return reinterpret_cast<T>(m_Original);
	}

private:
	void* m_Target{};
	void* m_Original{};
};
