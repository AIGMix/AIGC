#pragma once

#include <string>
#include <vector>

namespace aigc
{
    class ThreadPoolHelper
    {
    public:
        ThreadPoolHelper(int poolSize = 10);
        ~ThreadPoolHelper();

        int GetPoolSize();
        bool SetPoolSize(int newSize);
        bool AddWork();
        bool WaitAll(int msecond);

    private:
        int m_poolSize;
        std::vector<void*> m_threads;
    };
} // namespace aigc