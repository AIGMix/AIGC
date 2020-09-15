#pragma once
#include <string>

#ifdef _WIN32
    #include <winsock2.h>
    using SOCKET_AIGC = SOCKET;
#else
    #include <sys/socket.h>
    using SOCKET_AIGC = int;
#endif

namespace aigc
{

class SocketHelper
{
public:
    enum class Protocol
    {
        IPv4,
        IPv6,
    };

    /**
     * @brief 构造函数
     * @param protocol 网络协议
     */
    SocketHelper(Protocol protocol = Protocol::IPv4);
    ~SocketHelper();

    /**
     * @brief 获取AF_INET或AF_INET6
     * @param protocol 网络协议
     */
    static int GetAddressFamily(Protocol protocol);

    /**
     * @brief 连接
     * @param str ip地址或者域名
     * @param port 端口
     */
    bool Connect(std::string str, int port = 80, bool isDomain = false);

    /**
     * @brief 连接
     * @param address sockaddr格式信息
     * @param addressSize address大小
     */
    bool Connect(const struct sockaddr *address, int addressSize);

    /**
     * @brief 关闭连接
     */
    void CloseConnect();

    /**
     * @brief 绑定与监听
     * @param port 端口
     */
    bool Bind(int port);

    /**
     * @brief 发送数据
     * @param buffer 数据包
     * @param length 数据包大小
     * @param flags flags标志
     * @return 发送出去的数据包长度
     */
    int Send(const char* buffer, int length, int flags = 0);

    /**
     * @brief 接收数据
     * @param buffer 数据缓存
     * @param length 缓存大小
     * @param flags flags标志
     * @return 接收到的数据长度
     */
    int Receive(char *buffer, size_t length, int flags = 0);

    /**
     * @brief 获取失败信息
     * @param errorMessage 信息
     * @return error值
     */
    int GetLastError(std::string& errorMessage);

private:
    Protocol m_protocal;
    SOCKET_AIGC m_socket;

    int m_lastError;
    std::string m_lastErrorMessage;
};


}