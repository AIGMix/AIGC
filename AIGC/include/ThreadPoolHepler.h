#pragma once

#include <string>
#include <vector>
#include <thread>
#include <atomic>
#include <functional>

#include "TaskHelper.h"
#include "QueueHelper.h"

namespace aigc
{
    class ThreadPoolHelper
    {
    public:
        /**
         * @brief 构造函数
         * @param threadnum 线程的数量
         * @param taskCapacity 任务的容量上限
         */
        ThreadPoolHelper(int threadnum, int taskCapacity) : m_tasks(taskCapacity)
        {
            m_stop = false;
            m_threadnum = 0;
            SetThreadNum(threadnum);
        }

        ~ThreadPoolHelper()
        {
            Stop();
        }

        /**
         * @brief 获取线程的数量
         */
        int GetThreadNum()
        {
            return m_threadnum;
        }

        /**
         * @brief 设置线程的数量
         * @param threadnum 线程数
         */
        bool SetThreadNum(int threadnum)
        {
            if (threadnum >= m_threadnum)
            {
                int addnum = threadnum - m_threadnum;
                for (int i = 0; i < m_threadnum; ++i)
                {
                    m_threads.push_back(std::make_shared<std::thread>(&ThreadPoolHelper::CALL_BACK_FUNCTION, this));
                }
                m_threadnum = threadnum;
                return true;
            }
            
            return false;
        }

        /**
         * @brief 添加任务
         * @param task 任务
         */
        bool AddTask(Task task)
        {
            return m_tasks.Add(task);
        }

        /**
         * @brief 停止线程池
         */
        void Stop()
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

    private:
        /**
         * @brief 线程回调
         */
        void CALL_BACK_FUNCTION()
        {
            while (!m_stop)
            {
                Task task;
                if (!m_tasks.Get(task))
                    continue;

                task.func(task);
            }
        }

        int m_threadnum;
        std::atomic_bool m_stop;                           //是否停止的标志
        QueueHelper<Task> m_tasks;                         //任务队列队列
        std::list<std::shared_ptr<std::thread>> m_threads; //处理任务的线程组
    };

    

    

    

    

} // namespace aigc