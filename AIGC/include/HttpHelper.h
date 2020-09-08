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

    /**
    * @brief Get操作
    * @param url 链接
    */
    static Result Get(const std::string& url);

    /**
    * @brief Post操作
    * @param url 链接
    * @param data 数据
    * @param headers 头
    */
    static Result Post(const std::string& url, const std::map<std::string, std::string>& data = {}, const std::vector<std::string>& headers = {});
};
}


