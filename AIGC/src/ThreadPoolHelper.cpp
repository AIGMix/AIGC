#include "ThreadPoolHepler.h"

namespace aigc
{
    void ThreadPoolHelper::ThreadCallBack()
    {
        while (!m_stop)
        {
            Task task;
            if (!m_tasks.Get(task))
                continue;

            task.work(task.data);
            //work.func(work.data);
        }
    }

    ThreadPoolHelper::ThreadPoolHelper(int threadnum, int taskCapacity) : m_tasks(taskCapacity)
    {
        m_stop = false;
        m_threadnum = 0;
        SetThreadNum(threadnum);
    }

    ThreadPoolHelper::~ThreadPoolHelper()
    {
        Stop();
    }

    int ThreadPoolHelper::GetThreadNum()
    {
        return m_threadnum;
    }

    bool ThreadPoolHelper::SetThreadNum(int threadnum)
    {
        if (threadnum >= m_threadnum)
        {
            int addnum = threadnum - m_threadnum;
            for (int i = 0; i < m_threadnum; ++i)
            {
                m_threads.push_back(std::make_shared<std::thread>(&ThreadPoolHelper::ThreadCallBack, this));
            }
            m_threadnum = threadnum;
            return true;
        }
        else
            return false;
    }

    bool ThreadPoolHelper::AddTask(Task task)
    {
        return m_tasks.Add(task);
    }

    void ThreadPoolHelper::Stop()
    {
        if (m_stop)
            return;
        
        m_stop = true;
        m_tasks.Stop();               //让阻塞的线程都跳出等待
        for (auto thread : m_threads) //等待线程结束
        {
            if (thread)
                thread->join();
        }
        m_threads.clear();
        m_threadnum = 0;
    }
}

