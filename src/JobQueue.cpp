#include "pch.h"
#include "JobQueue.h"

void JobQueue::Add(job_t job)
{
	m_list.push(job);
}

void JobQueue::Run()
{
	while (!m_list.empty())
	{
		m_list.front()();
		m_list.pop();
	}
}
