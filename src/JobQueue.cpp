#include "pch.h"
#include "JobQueue.h"

void JobQueue::Add(job_t Job)
{
	m_Queue.push(Job);
}

void JobQueue::Run()
{
	while (!m_Queue.empty())
	{
		TRY
		{
			m_Queue.front()();
		}
		EXCEPT{ LOG_EXCEPTION(); }
		m_Queue.pop();
	}
}
