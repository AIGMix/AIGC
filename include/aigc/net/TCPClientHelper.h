#pragma once

#include "aigc/net/SocketHelper.h"
#include "aigc/system/RWLockHelper.h"
#include "aigc/common/StringHelper.h"

namespace aigc
{
    class TCPClientHelper : SocketHelper
    {
    public:
        TCPClientHelper(const std::string &host = "", int port = 0)
        {
            m_clientPort = 0;
            m_isConnected = false;
            if (host != "")
                Connect(host, port);
        }

        ~TCPClientHelper()
        {
            CloseConnect();
        }

    public:
        /**
         * @brief 连接服务器
         * @param host 域名IP
         * @param port 端口
         */
        bool Connect(const std::string &host, int port)
        {
            ShareRWLock lock(&m_rwMutex, true);

            if (m_isConnected)
            {
                SocketClose(m_socket);
                m_isConnected = false;
            }

            //创建socket
            if (!SocketCreat(host, port))
                return false;

            struct sockaddr_in serverAddress;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = inet_addr(m_ip.c_str());
            serverAddress.sin_port = htons(m_port);

            //连接
            if (0 > connect(m_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)))
            {
                SocketClose(m_socket);
                SetLastError("Connect failed!");
                return false;
            }

            //获取客户端自己的本地IP和端口
            m_clientPort = NetHelper::GetPortBySocket(m_socket);
            m_isConnected = true;
            return true;
        }

        /**
         * @brief 关闭连接
         */
        void CloseConnect()
        {
            ShareRWLock lock(&m_rwMutex, true);
            SocketClose(m_socket);
            m_isConnected = false;
        }

        /**
         * @brief 查询是否连接
         */
        bool IsConnected()
        {
            ShareRWLock lock(&m_rwMutex, false);
            return m_isConnected;
        }

    public:
        /**
         * @brief 发送数据
         * @return 发送出去的数据包长度
         */
        int Send(const char *buffer, int length)
        {
            ShareRWLock lock(&m_rwMutex, false);
            if (!m_isConnected)
            {
                SetLastError("UnConnect!");
                return -1;
            }
            return SocketSend(m_socket, buffer, length);
        }

        int Send(const std::string &str)
        {
            return Send(str.c_str(), str.length());
        }

    public:
        /**
         * @brief 接收数据
         * @param buffer 数据缓存
         * @param length 缓存大小
         * @param msTimeout 毫秒超时
         * @return 接收到的数据长度
         */
        int Receive(char *buffer, size_t length, int msTimeout = 0)
        {
            ShareRWLock lock(&m_rwMutex, false);
            if (!m_isConnected)
            {
                SetLastError("UnConnect!");
                return -1;
            }
            if (msTimeout > 0)
            {
                fd_set fdSet;
                if (!SocketSelect(fdSet, { m_socket }, msTimeout))
                    return 0;
            }
            return SocketReceive(m_socket, buffer, length);
        }

        /**
         * @brief 接收数据
         * @param maxBufferLength 接收缓存的最大长度
         * @param msTimeout 超时时间
         */
        std::string Receive(int maxBufferLength, int msTimeout = 0)
        {
            if (maxBufferLength <= 0)
                return "";

            //缓存
            std::shared_ptr<char> buffer(new char[maxBufferLength], std::default_delete<char[]>());
            char *pbuffer = buffer.get();
            int len = Receive(pbuffer, maxBufferLength, msTimeout);
            if (len <= 0)
                return "";
            pbuffer[len] = '\0';
            std::string ret = std::string(pbuffer);
            return ret;
        }

    public:
        std::string GetLastErrorMessage(int *errorNum = NULL)
        {
            return ((SocketHelper *)this)->GetLastErrorMessage(errorNum);
        }

    private:
        bool m_isConnected;
        int m_clientPort;
        RWLockHelper m_rwMutex; 
    };

    typedef std::shared_ptr<TCPClientHelper> TCPClientHelperPtr;

} // namespace aigc