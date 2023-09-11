#pragma once

#include "pch.h"

class Timer
{
public:
	inline float GetMillis() const
	{
		auto Now = std::chrono::high_resolution_clock::now();
		auto Milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(Now - m_Start).count();
		auto Microseconds = std::chrono::duration_cast<std::chrono::microseconds>(Now - m_Start).count() % 1000;

		return (float)Milliseconds + (float)Microseconds / 1000.0f;
	}

public:
	inline static float s_ESPTime{}, s_ScriptThreadTime{}, s_JobQueueTime{}, s_MenuTime{}, s_RenderThreadTime{}, s_ScriptThreadTickTime{};

private:
	std::chrono::high_resolution_clock::time_point m_Start = std::chrono::high_resolution_clock::now();
};
