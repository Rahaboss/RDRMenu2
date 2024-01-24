#pragma once

#include "pch.h"
#include "Rage/joaat.h"

namespace Thread
{
	void ExecuteAsThread(rage::joaat_t ScriptHash, void (*Function)());
	void YieldThread();
	void YieldThread(size_t Milliseconds);

	template<class _Rep, class _Period>
	inline void YieldThread(const std::chrono::duration<_Rep, _Period>& Duration)
	{
		const std::chrono::time_point EndTime = std::chrono::system_clock::now() + Duration;
		do
		{
			YieldThread();
		} while (std::chrono::system_clock::now() < EndTime);
	}
}
