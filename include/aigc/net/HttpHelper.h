#pragma once

#include "aigc/net/HttpRequestHelper.h"
#include "aigc/common/JsonHelper.h"

//#ifdef _WIN32
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#else
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <netdb.h>
//#include <unistd.h>
//#include <errno.h>
//#endif

namespace aigc
{
    class HttpHelper 
    {
    public:
        struct Result
        {
            int status;
            std::string message;
            strings headers;
            std::string data;
            std::vector<char> cdata;
        };

        static Result Request(const std::string& url,
			                    const std::string& method = "GET",
                                const std::map<std::string, std::string>& parameters = {},
			                    const strings& headers = {})
        {
			HttpRequestHelperPtr request = std::make_shared<HttpRequestHelper>(url);
			request->method = method;
			request->paras = parameters;
			request->headers = headers;

            Result result = {0};
            if (!request->SendRequest())
            {
                result.status = request->status;
                result.message = request->message;
                return result;
            }

			HttpResponseHelperPtr response = request->GetResponse();
            if (response == NULL)
			{
				result.status = request->status;
				result.message = request->message;
				return result;
			}

            char buffer[1024];
            while (true)
            {
                int size = response->Read(buffer, 1024);
                if (size < 0)
                {
					result.status = response->status;
					result.message = response->message;
                    return result;
                }
                else if (size == 0)
                    break;
                result.cdata.insert(result.cdata.end(), buffer, buffer + size);
            }

            result.headers = response->headers;
            result.data = std::string(result.cdata.begin(), result.cdata.end());
            return result;
        }

    public:

        static std::string GetString(const std::string& url,
                                    const std::map<std::string, std::string>& parameters = {},
                                    const std::vector<std::string>& headers = {})
        {
			Result result = Request(url, "GET", parameters, headers);
			if (!result.message.empty())
				return "";
            return result.data;
        }

        /**
         * @brief Get操作-获取Json对象，并直接转换成为类
         * @param obj 类对象
         * @param url 链接
         * @param parameters 参数集合
         * @param headers 头
         * @param keys json项的key数组
         */
        template <typename T>
        static bool GetJsonObject(T &obj,
                                  const std::string &url,
                                  const std::map<std::string, std::string> &parameters = {},
                                  const std::vector<std::string> &headers = {},
                                  const std::vector<std::string> &keys = {})
        {
            std::string json = GetString(url, parameters, headers);
            if (json.empty())
                return false;
            return JsonHelper::JsonToObject(obj, json, keys);
        }

    };
}


