#pragma once

#include <map>
#include <vector>
#include <string>

namespace aigc
{
class HttpHelper 
{

public:
    typedef struct UrlStruct
    {
        std::string scheme;  //http或https
        std::string domain;  //域名
        std::string port;    //端口
        std::string path;    //路径
    }Url;

    typedef struct ResponseStruct
    {
        int status;
        bool success;
        std::string errMessage;
        std::string dataStr;
        std::vector<char> dataBytes;
        std::vector<std::string> headers;
    } Response;

    /**
     * @brief 获取解析后的链接
     * @param url 链接
     */
    static Url GetUrl(const std::string& url);

    /**
     * @brief Url编码
     * @param str 要编码的字符串
     */
    static std::string UrlEncode(const std::string &str);

    /**
     * @brief 发送请求
     * @param url 链接
     * @param method GET/POST
     * @param body 内容
     * @param headers 头
     */
    static Response GetResponse(const std::string &url,
                                const std::string method,
                                const std::string &body,
                                const std::vector<std::string> &headers = {});

    /**
     * @brief 发送请求
     * @param url 链接
     * @param method GET/POST
     * @param parameters 参数集合
     * @param headers 头
     */
    static Response GetResponse(const std::string &url,
                                const std::string method,
                                const std::map<std::string, std::string> &parameters,
                                const std::vector<std::string> &headers = {});


    static Response Get(const std::string &url,
                        const std::map<std::string, std::string> &parameters,
                        const std::vector<std::string> &headers = {})
    {
        return GetResponse(url, "GET", parameters, headers);
    }

    static Response Post(const std::string &url,
                         const std::map<std::string, std::string> &parameters,
                         const std::vector<std::string> &headers = {})
    {
        return GetResponse(url, "POST", parameters, headers);
    }
};
}


