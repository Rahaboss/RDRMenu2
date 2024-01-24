#include "pch.h"
#include "JobQueue.h"
#include "Util/Timer.h"
#include "Fiber.h"

static std::list<Fiber*> s_Queue;
void JobQueue::Add(Job_t Job)
{
	s_Queue.push_back(new Fiber{ Job, &Fiber::JobQueueFiberFunction });
}

void JobQueue::Run()
{
	Timer JobQueueTimer;

	auto it = s_Queue.cbegin();
	while (it != s_Queue.cend())
	{
		Fiber* f = *it;

		bool Done = false;

		f->m_ExtraData = &Done;
		f->Tick();

		if (Done)
		{
			delete f;
			it = s_Queue.erase(it);
		}
		else
			++it;
	}

	Timer::s_JobQueueTime = JobQueueTimer.GetMillis();
}

size_t JobQueue::GetJobsQueued()
{
	return s_Queue.size();
}
