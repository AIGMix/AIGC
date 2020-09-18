#include "QueueHelper.h"

namespace aigc
{
    template <typename T>
    QueueHelper<T>::QueueHelper(int size)
    {
        m_maxSize = size;
        m_stop = false;
    }

    template <typename T>
    bool QueueHelper<T>::Add(T data)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notFull.wait(locker, [this] { return m_stop || (m_queue.size() < m_maxSize); });
        if (m_stop)
            return false;

        m_queue.push_back(std::forward<T>(data));
        m_notEmpty.notify_one();
        return true;
    }

    template <typename T>
    bool QueueHelper<T>::Get(T &data)
    {
        std::unique_lock<std::mutex> locker(m_mutex);
        m_notEmpty.wait(locker, [this] { return m_stop || (m_queue.size() > 0); });
        if (m_stop)
            return false;

        data = m_queue.front();
        m_queue.pop_front();
        m_notFull.notify_one();
        return true;
    }

    template <typename T>
    void QueueHelper<T>::Stop()
    {
        { //lock_guard的生命周期
            std::lock_guard<std::mutex> locker(m_mutex);
            m_stop = true;
        }
        m_notFull.notify_all();
        m_notEmpty.notify_all();
    }

    template <typename T>
    int QueueHelper<T>::GetSize()
    {
        std::lock_guard<std::mutex> locker(m_mutex);
        return m_queue.size();
    }

}

#include "TaskHelper.h"
void TaskFunc(aigc::TaskData data)
{

}

int main()
{
    aigc::QueueHelper<aigc::Task> queue(10);
    aigc::Task tt;
    tt.work = TaskFunc;
    queue.Add(tt);
    queue.Get(tt);
    queue.Stop();
    return 0;
}