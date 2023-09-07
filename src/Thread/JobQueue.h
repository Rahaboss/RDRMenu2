#pragma once

#include "pch.h"

namespace JobQueue
{
	typedef std::function<void()> Job_t;

	void Add(Job_t Job);
	void Run();
	size_t GetJobsQueued();
}

#define QUEUE_JOB(...)\
	JobQueue::Add([__VA_ARGS__]()\
	{\
		__try

#define END_JOB()\
		__except (EXCEPTION_EXECUTE_HANDLER)\
		{\
			LOG_EXCEPTION();\
		}\
	});
