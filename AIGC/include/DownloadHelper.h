#pragma once

#include <string>
#include "ThreadPoolHelper.h"

namespace aigc
{
    
    class DownloadHelper
    {
    public:
        static bool Start(const std::string &url, const std::string &path, int threadNum);
        static bool Start(const std::string &url, const std::string &path, ThreadPoolHelper &pool);
    };

} // namespace aigc
