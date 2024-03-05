#pragma once

#include "pch.h"

class Timer
{
public:
#if _DIST
	inline constexpr float GetMillis() const { return 0; }
#else
	inline float GetMillis() const
	{
		std::chrono::time_point Now = std::chrono::steady_clock::now();
		return static_cast<float>(std::chrono::duration_cast<std::chrono::microseconds>(Now - m_Start).count()) / 1000.0f;
	}

private:
	std::chrono::steady_clock::time_point m_Start = std::chrono::steady_clock::now();
#endif // _DIST

public:
	inline static float s_ESPTime{}, s_ScriptThreadTime{}, s_JobQueueTime{}, s_MenuTime{}, s_RenderThreadTime{}, s_ScriptThreadTickTime{};
};
