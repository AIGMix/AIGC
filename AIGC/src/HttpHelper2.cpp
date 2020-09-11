#include "HttpHelper.h"
#include "SocketHelper.h"
#include "StringHelper.h"
#include <memory>
#include <algorithm>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <netdb.h>
    #include <unistd.h>
    #include <errno.h>
#endif

namespace aigc
{
    static char S_CRLF[] = {'\r', '\n'};
    class ReceiveHeader
    {
    public:
        //状态
        int status = 0;
        //第一行(临时使用)
        bool firstLine = false;
        //内容长度
        unsigned long contentLength = -1;
        //内容长度不确定 chunked编码
        bool chunkedResponse = false;
        //头集合
        std::vector<std::string> headers;
        //错误信息
        bool error = false;
        std::string errMessage;
    };

    HttpHelper::Url HttpHelper::GetUrl(const std::string &url)
    {
        Url ret;
        ret.path = url;
        ret.port = "80";
        ret.scheme = "http";

        //获取格式
        size_t pos = url.find("://");
        if (pos != std::string::npos)
        {
            ret.scheme = url.substr(0, pos);
            ret.path = url.substr(pos + 3);
        }
        //去除位置片段
        pos = ret.path.find('#');
        if (pos != std::string::npos)
            ret.path.resize(pos);
        //获取域名
        pos = ret.path.find('/');
        if (pos == std::string::npos)
        {
            ret.domain = ret.path;
            ret.path = "/";
        }
        else
        {
            ret.domain = ret.path.substr(0, pos);
            ret.path = ret.path.substr(pos);
        }
        //获取端口
        pos = ret.domain.find(':');
        if (pos != std::string::npos)
        {
            ret.port = ret.domain.substr(pos + 1);
            ret.domain.resize(pos);
        }
        return ret;
    }

    std::string HttpHelper::UrlEncode(const std::string &str)
    {
        std::string result;
        constexpr char hexChars[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

        for (auto i = str.begin(); i != str.end(); ++i)
        {
            const std::uint8_t cp = *i & 0xFF;
            
            if ((cp >= 0x30 && cp <= 0x39) ||           // 0-9
                (cp >= 0x41 && cp <= 0x5A) ||           // A-Z
                (cp >= 0x61 && cp <= 0x7A) ||           // a-z
                cp == 0x2D || cp == 0x2E || cp == 0x5F) // - . _
                result += static_cast<char>(cp);
            else if (cp <= 0x7F) // length = 1
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
            else if ((cp >> 5) == 0x06) // length = 2
            {
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                if (++i == str.end())
                    break;
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
            }
            else if ((cp >> 4) == 0x0E) // length = 3
            {
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                if (++i == str.end())
                    break;
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                if (++i == str.end())
                    break;
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
            }
            else if ((cp >> 3) == 0x1E) // length = 4
            {
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                if (++i == str.end())
                    break;
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                if (++i == str.end())
                    break;
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                if (++i == str.end())
                    break;
                result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
            }
        }

        return result;
    }

    HttpHelper::Result HttpHelper::Send(const std::string &url,
                                        const std::string method,
                                        const std::map<std::string, std::string> &parameters,
                                        const std::vector<std::string> &headers,
                                        bool isIPv6)
    {
        std::string body;
        bool first = true;
        for (const auto &parameter : parameters)
        {
            if (!first)
                body += "&";
            first = false;
            body += UrlEncode(parameter.first) + "=" + UrlEncode(parameter.second);
        }
        return Send(url, method, body, headers, isIPv6);
    }

    /**
     * @brief 解析头
     * @param receiveHeader 临时头
     * @param data 接收到的数据
     * @return True 解析完毕
     */
    static bool ParseHeaders(ReceiveHeader &receiveHeader, std::vector<char>& data)
    {
        char crlf[] = {'\r', '\n'};
        while(true)
        {
            const auto iter = std::search(data.begin(), data.end(), std::begin(S_CRLF), std::end(S_CRLF));
            if (iter == data.end())
                break;

            //获取此行并截取data
            const std::string line(data.begin(), iter);
            data.erase(data.begin(), iter + 2);

            //空行表示header部分的结尾
            if (line.empty())
                return true;

            //第一行，例子 "Http/1.1 202 OK"
            if (receiveHeader.firstLine)
            {
                receiveHeader.firstLine = false;
                std::vector<std::string> array = StringHelper::Split(line, ' ');
                if (array.size() >= 2)
                    receiveHeader.status = std::stoi(array[1]);
                continue;
            }

            //获取每一行的参数
            receiveHeader.headers.push_back(line);
            const auto pos = line.find(':');
            if (pos == std::string::npos)
                continue;

            std::string name = line.substr(0, pos);
            std::string value = StringHelper::Trim(line.substr(pos + 1), " ");
            
            //获取内容长度
            if (name == "Content-Length")
                receiveHeader.contentLength = std::stoul(value);
            //内容长度不能确定
            else if (name == "Transfer-Encoding")
            {
                if (value == "chunked")
                    receiveHeader.chunkedResponse = true;
                else
                {
                    receiveHeader.error = true;
                    receiveHeader.errMessage = "Unsupported transfer encoding: " + value;
                    return false;
                }
            }
        }
        return false;
    }

    class ChunkedParas
    {
    public:
        //等待接收的子串剩余大小
        int expectedChunkSize = 0;
        //上一个子串已结束
        bool lastChunkEnd = false;
    };

    /**
     * @brief 通过Chunked编码接收主体数据
     * @param retData 接收后的数据
     * @param data Chunked编码的数据
     * @param chunkedParas chunked解析时的临时参数
     * @return True 主体接收完成
     */
    static bool ReceiveByChunked(std::vector<char> &retData, std::vector<char> &data, ChunkedParas &chunkedParas)
    {
        /**
         * Chunked编码使用若干个Chunk串连而成，由一个标明长度为0的chunk标示结束。
         * 每个Chunk分为头部和正文两部分，头部内容指定下一段正文的字符总数（十六进制的数字）和数量单位（一般不写），
         * 正文部分就是指定长度的实际内容，两部分之间用回车换行(CRLF)隔开。
         * 在最后一个长度为0的Chunk中的内容是称为footer的内容，是一些附加的Header信息（通常可以直接忽略）
         * 
         * 格式：
         * [chunk size][\r\n][chunk data][\r\n][chunk size][\r\n][chunk data][\r\n][chunk size = 0][\r\n][\r\n]
         */
        bool dataReceived = false;
        while (true)
        {
            if (chunkedParas.expectedChunkSize > 0)
            {   
                //获取待写入的大小
                int size = chunkedParas.expectedChunkSize < data.size() ? chunkedParas.expectedChunkSize : data.size();
                retData.insert(retData.end(), data.begin(), data.begin() + static_cast<ptrdiff_t>(size));

                //裁剪
                data.erase(data.begin(), data.begin() + static_cast<ptrdiff_t>(size));
                chunkedParas.expectedChunkSize -= size;

                if (chunkedParas.expectedChunkSize == 0)
                    chunkedParas.lastChunkEnd = true;
                if (data.empty())
                    break;
            }
            else
            {   
                //去除上一个子串后面的\r\n
                if (chunkedParas.lastChunkEnd)
                {
                    if (data.size() < 2)
                        break;
                    chunkedParas.lastChunkEnd = false;
                    data.erase(data.begin(), data.begin() + 2);
                }

                //查找此子串的\r\n
                const auto pos = std::search(data.begin(), data.end(), std::begin(S_CRLF), std::end(S_CRLF));
                if (pos == data.end())
                    break;

                const std::string size(data.begin(), pos);
                data.erase(data.begin(), pos + 2);

                chunkedParas.expectedChunkSize = std::stoul(size, nullptr, 16);
                //如果长度为0表示已经接收完毕
                if (chunkedParas.expectedChunkSize == 0)
                    return true;
            }
        }
        return false;
    }

    HttpHelper::Result HttpHelper::Send(const std::string &inUrl,
                                        const std::string method,
                                        const std::string &inBody,
                                        const std::vector<std::string> &headers,
                                        bool isIPv6)
    {
        Result result;
        Url url = GetUrl(inUrl);
        std::vector<uint8_t> body = std::vector<uint8_t>(inBody.begin(), inBody.end());

        //连接
        SocketHelper socket(isIPv6 ? SocketHelper::Protocol::IPv6 : SocketHelper::Protocol::IPv4);
        if (socket.Connect(url.domain, url.port) == false)
        {
            result.success = false;
            socket.GetLastError(result.errMessage);
            return result;
        }

        //组装头数据
        std::string headerData = method + " " + url.path + " HTTP/1.1\r\n";
        for (const std::string &header : headers)
            headerData += header + "\r\n";
        headerData += "Host: " + url.domain + "\r\nContent-Length: " + std::to_string(body.size()) + "\r\n\r\n";

        //组装请求数据
        std::vector<uint8_t> requestData(headerData.begin(), headerData.end());
        requestData.insert(requestData.end(), body.begin(), body.end());

        //发送请求数据
        auto remaining = requestData.size();
        auto sendData = requestData.data();
        while (remaining > 0)
        {
            const auto size = socket.Send((char*)sendData, remaining);
            remaining -= size;
            sendData += size;
        }

        //接收数据
        char buffer[4096];
        int bufferSize = sizeof(buffer);
        std::vector<char> data;
        std::vector<char> retData;
        bool haveExpand = false;
        bool headerOver = false;
        ReceiveHeader receiveHeader;
        ChunkedParas chunkedParas;

        while(true)
        {
            int size = socket.Receive(buffer, bufferSize);
            if (size <= 0)
                break;

            data.insert(data.end(), buffer, buffer + size);

            //解析头
            if (headerOver == false)
            {
                headerOver = ParseHeaders(receiveHeader, data);
                if (receiveHeader.error == true)
                {
                    result.success = false;
                    result.errMessage = receiveHeader.errMessage;
                    result.status = receiveHeader.status;
                    result.headers = receiveHeader.headers;
                    return result;
                }
                if (headerOver == false)
                    continue;
            }

            //装载数据
            if (receiveHeader.chunkedResponse == false)
            {
                if (haveExpand == false)
                {
                    retData.reserve(receiveHeader.contentLength);
                    haveExpand = true;
                }
                retData.insert(retData.end(), data.begin(), data.end());
                data.clear();
                //判断是否已经接收完成
                if (receiveHeader.contentLength >= 0 && retData.size() >= receiveHeader.contentLength)
                    break;
            }
            //装载chunked编码的数据
            else
            {
                if(ReceiveByChunked(retData, data, chunkedParas))
                    break;
            }
        }

        result.dataBytes = retData;
        result.dataStr = std::string(retData.begin(), retData.end());
        result.success = true;
        result.errMessage = "";
        result.status = receiveHeader.status;
        result.headers = receiveHeader.headers;
        return result;
    }
}

int main()
{
    aigc::HttpHelper::Result ret = aigc::HttpHelper::Send("www.baidu.com", "GET", "");
    return 0;
}