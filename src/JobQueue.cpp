#include "pch.h"
#include "JobQueue.h"

void JobQueue::Add(job_t job)
{
	m_List.push(job);
}

void JobQueue::Run()
{
	while (!m_List.empty())
	{
		m_List.front()();
		m_List.pop();
	}
}
