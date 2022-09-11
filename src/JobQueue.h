#pragma once

#include "pch.h"
#include "Features.h"

typedef std::function<void()> job_t;

class JobQueue
{
public:
	void Add(job_t job);
	void Run();

private:
	std::queue<job_t> m_list;
};

inline JobQueue g_JobQueue;

#define QUEUE_JOB(...) g_JobQueue.Add([__VA_ARGS__]() { __try
#define END_JOB __except (EXCEPTION_EXECUTE_HANDLER) { std::cout << "Caught exception in queued job:\n\tFile " << __FILE__ << ":" << __LINE__ << '\n'; } });
