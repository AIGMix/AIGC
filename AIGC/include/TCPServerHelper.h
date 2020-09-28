#pragma once

#include <string>

namespace aigc
{
    class TCPServerHelper
    {
    public:
        bool Start(std::string host, std::string port);


    };
} // namespace aigc