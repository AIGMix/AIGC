#pragma once

#include <map>
#include <vector>
#include <string>

namespace aigc
{
class HttpHelper
{

public:
    typedef struct ResultStruct
    {
        bool success;
        std::string errMessage;
        std::string dataStr;
        std::vector<std::uint8_t> dataBytes;
    }Result;

    static Result Get(const std::string& url);
    static Result Post(const std::string& url, const std::map<std::string, std::string>& data = {}, const std::vector<std::string>& headers = {});
};
}


