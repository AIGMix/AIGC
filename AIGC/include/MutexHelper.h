#pragma once

#include <string>

namespace aigc
{
    class MutexHelper
    {
    public:
        MutexHelper(const std::string& name = "");
        ~MutexHelper();

        bool Creat(const std::string& name = "");
        bool Open(const std::string& name);
        bool Close();

        bool Lock(int msecond);
        bool Release();

    private:
        void* m_mutex;
    };

} 
