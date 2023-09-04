#include "pch.h"
#include "JobQueue.h"

static std::queue<JobQueue::Job_t> s_Queue;
void JobQueue::Add(Job_t Job)
{
	s_Queue.push(Job);
}

void JobQueue::Run()
{
	while (!s_Queue.empty())
	{
		TRY
		{
			s_Queue.front()();
		}
		EXCEPT{ LOG_EXCEPTION(); }
		s_Queue.pop();
	}
}
