#pragma once

#include <string>

namespace aigc
{
    class AESHelper
    {
    public:
        static std::string Encode(const std::string &str);
    };

} // namespace aigc
