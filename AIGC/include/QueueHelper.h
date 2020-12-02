#pragma once

#include <string>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable>

namespace aigc
{

template <typename T>
class QueueHelper
{
public:
    QueueHelper(int maxSize)
    {
        m_maxSize = size;
        m_stop = false;
    }

    /**
     * @brief 添加项（阻塞）
     * @param data 项
     */
    bool Add(T data)
    {
        m_notFull.wait(locker, [this] { return m_stop || (m_queue.size() < m_maxSize); });
        if (m_stop)
            return false;

        std::unique_lock<std::mutex> locker(m_mutex);
        m_queue.push_back(std::forward<T>(data));
        m_notEmpty.notify_one();
        return true;
    }

    /**
     * @brief 获取项（阻塞）
     * @param data 项
     */
    bool Get(T &data)
    {
        m_notEmpty.wait(locker, [this] { return m_stop || (m_queue.size() > 0); });
        if (m_stop)
            return false;

        std::unique_lock<std::mutex> locker(m_mutex);
        data = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
        return true;
    }
    
    /**
     * @brief 停止
     */
    void Stop()
    {
        m_stop = true;
        m_notFull.notify_all();
        m_notEmpty.notify_all();
    }

    /**
     * @brief 获取队列当前大小
     */
    int GetSize()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

private:
    std::list<T> m_queue;               //缓冲区
    int m_maxSize;                      //同步队列最大的size
    bool m_stop;                        //停止的标志

    std::mutex m_mutex;                 //互斥量和条件变量结合起来使用
    std::condition_variable m_notEmpty; //不为空的条件变量
    std::condition_variable m_notFull;  //没有满的条件变量
};

} // namespace aigc
