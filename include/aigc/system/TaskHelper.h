#pragma once

#include <string>
#include <vector>
#include <functional>

namespace aigc
{
    class Task
    {
    public:
        std::function<void(Task&)> func;

    public:
        void *dataPtr;
        int   dataInt;
        std::vector<std::string> dataStrings;
    };
}
