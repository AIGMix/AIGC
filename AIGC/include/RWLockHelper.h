#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <mutex>


namespace aigc
{
    class RWLockHelper
    {
    public:
        RWLockHelper()
        {
            m_readerNum = 0;
        }

        void readLock()
        {
            m_rlock.lock();
            m_readerNum++;
            if (m_readerNum == 1)
                m_wlock.lock();
            m_rlock.unlock();
        }

        void readUnlock()
        {
            m_rlock.lock();
            m_readerNum--;
            if (m_readerNum == 0)
                m_wlock.unlock();
            else if (m_readerNum < 0)
                m_readerNum = 0;
            m_rlock.unlock();
        }

        void writeLock()
        {
            m_wlock.lock();
        }

        void writeUnlock()
        {
            m_wlock.unlock();
        }
    private:
        int m_readerNum;
        std::mutex m_rlock;
        std::mutex m_wlock;
    };

    class ShareRWLock
    {
    public:
        ShareRWLock(RWLockHelper *lock, bool isWrite = false)
        {
            m_lock = lock;
            m_isWrite = isWrite;
            if (isWrite)
                m_lock->writeLock();
            else
                m_lock->readLock();
        }

        ~ShareRWLock()
        {
            if (m_isWrite)
                m_lock->writeUnlock();
            else
                m_lock->readUnlock();
        }

    private:
        bool m_isWrite;
        RWLockHelper* m_lock;
    };
} // namespace aigc
