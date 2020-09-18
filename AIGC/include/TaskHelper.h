#pragma once

#include <string>
#include <functional>

namespace aigc
{
    typedef struct TaskDataStruct
    {
        int ivalue;
        void* pvalue;
        std::string svalue;
    }TaskData;

    typedef struct TaskStruct
    {
        std::function<void(TaskData&)> work;
        TaskData data;
    }Task;
}
