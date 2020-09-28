#pragma once
#include <string>



namespace aigc
{

class SocketHelper
{
public:
    SocketHelper(std::string hostOrIP = "127.0.0.1", int port = 80);
    ~SocketHelper();

    bool Connect();
    void CloseConnect();
    bool Bind();
    bool Accept();
    void SetIPAndPort(std::string hostOrIP = "127.0.0.1", int port = 80);

    /**
     * @brief 发送数据
     * @param buffer 数据包
     * @param length 数据包大小
     * @return 发送出去的数据包长度
     */
    int Send(const char* buffer, int length);

    /**
     * @brief 接收数据
     * @param buffer 数据缓存
     * @param length 缓存大小
     * @param mstimeout 毫秒超时
     * @return 接收到的数据长度
     */
    int Receive(char *buffer, size_t length, int mstimeout = 0);

    /**
     * @brief 获取失败信息
     * @param errorMessage 信息
     * @return error值
     */
    int GetLastError(std::string& errorMessage);

private:
    void SetLastError(std::string errorMessage);
    std::string GetIPByHost(std::string host);

    int m_socket;           //套接字
    int m_port;             //端口号
    std::string m_ip;       //IP地址
    std::string m_host;     //域名

    int m_lastError;                //最后的错误
    std::string m_lastErrorMessage; //最后的错误信息
};


}