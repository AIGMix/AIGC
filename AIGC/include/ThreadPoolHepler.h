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
    ThreadPoolHelper(int threadnum = 10, int taskCapacity = 100);

    /**
     * @brief 析构函数
     */
    ~ThreadPoolHelper();

    /**
     * @brief 获取线程的数量
     */
    int  GetThreadNum();

    /**
     * @brief 设置线程的数量
     * @param threadnum 线程数
     */
    bool SetThreadNum(int threadnum);

    /**
     * @brief 添加任务
     * @param task 任务
     */
    bool AddTask(Task task);

    /**
     * @brief 停止线程池
     */
    void Stop();

private:
    /**
     * @brief 线程回调
     */
    void ThreadCallBack();

    int m_threadnum;
    std::atomic_bool m_stop;                           //是否停止的标志
    QueueHelper<Task> m_tasks;                         //任务队列队列
    std::list<std::shared_ptr<std::thread>> m_threads; //处理任务的线程组
};

} // namespace aigc