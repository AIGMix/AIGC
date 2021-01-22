#pragma once

#include <string>
#include <mutex>
#include <memory>

namespace aigc
{

    template <class T>
    class GlobalHelper
    {

    public:
        static T *Instance()
        {
            if (nullptr == m_instance)
            {
                std::unique_lock<std::mutex> locker(m_instanceMutex);
                if (nullptr == m_instance)
                    m_instance.reset(new T);
            }
            return m_instance.get();
        }

    private:
        static std::mutex m_instanceMutex;
        static std::unique_ptr<T> m_instance;
    };

    template <class T>
    std::mutex GlobalHelper<T>::m_instanceMutex;
    template <class T>
    std::unique_ptr<T> GlobalHelper<T>::m_instance(nullptr);

} // namespace aigc
