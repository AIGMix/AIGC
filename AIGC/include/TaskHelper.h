#pragma once

#include <string>
#include <functional>

namespace aigc
{
    class Task
    {
    public:
        std::function<void(Task&)> func;

    public:
        int dataInt;
        char *dataPtr;
        std::string dataString;
    };
}
