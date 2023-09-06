#pragma once

#include "pch.h"

class Timer
{
public:
	inline float GetMillis()
	{
		auto now = std::chrono::high_resolution_clock::now();
		auto milliseconds_elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_Start).count();
		auto microseconds_elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - m_Start).count() % 1000;

		return (float)milliseconds_elapsed + (float)microseconds_elapsed / 1000.0f;
	}

public:
	inline static float s_ESPTime{}, s_ScriptThreadTime{}, s_JobQueueTime{};

private:
	std::chrono::high_resolution_clock::time_point m_Start = std::chrono::high_resolution_clock::now();
};
