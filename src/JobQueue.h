#pragma once

#include "pch.h"

typedef std::function<void()> job_t;

class JobQueue
{
public:
	void Add(job_t Job);
	void Run();

private:
	std::queue<job_t> m_Queue;
};

inline JobQueue g_JobQueue;

#define QUEUE_JOB(...) g_JobQueue.Add([__VA_ARGS__]() { __try
#define END_JOB() __except (EXCEPTION_EXECUTE_HANDLER) { LOG_TO_CONSOLE("Caught exception in queued job:\n\tFile %s:%d\n", __FILE__, __LINE__); } });
