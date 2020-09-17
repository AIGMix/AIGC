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
    QueueHelper(int size);

    /**
     * @brief 添加项（阻塞）
     * @param data 项
     */
    bool Add(T data);

    /**
     * @brief 获取项（阻塞）
     * @param data 项
     */
    bool Get(T &data);
    
    /**
     * @brief 停止
     */
    void Stop();

    /**
     * @brief 获取队列当前大小
     */
    int GetSize();

private:
    std::list<T> m_queue;               //缓冲区
    int m_maxSize;                      //同步队列最大的size
    bool m_stop;                        //停止的标志

    std::mutex m_mutex;                 //互斥量和条件变量结合起来使用
    std::condition_variable m_notEmpty; //不为空的条件变量
    std::condition_variable m_notFull;  //没有满的条件变量
};

} // namespace aigc
