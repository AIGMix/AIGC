#pragma once

#include "SocketHelper.h"
#include "RWLockHelper.h"

namespace aigc
{
    class TCPClientHelper : SocketHelper
    {
    public:
        TCPClientHelper(const std::string &host = "", int port = 0)
        {
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
            CloseConnect();
            ShareRWLock lock(&m_rwMutex, true);

            //创建socket
            if (!SocketCreat(host, port))
                return false;

            if (StringHelper::IsEmpty(m_ip))
            {
                SocketClose(m_socket);
                SetLastError("IP is empty!");
                return false;
            }

            struct sockaddr_in serverAddress;
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_addr.s_addr = inet_addr(m_ip.c_str());
            serverAddress.sin_port = htons(m_port);

            //连接
            if (0 > connect(m_socket, (struct sockaddr *)&serverAddress, sizeof(serverAddress)))
            {
                SocketClose(m_socket);
                SetLastError("Connect Failed!");
                return false;
            }

            //获取客户端自己的本地IP和端口
            m_clientPort = GetSelfPortBySocket(m_socket);

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
            if (buffer == NULL || length <= 0)
            {
                SetLastError("Input buffer error!");
                return -1;
            }

            int result = send(m_socket, buffer, length, 0);
            if (result <= 0)
            {
                SetLastError("Send failed!");
                return -1;
            }
            return result;
        }

        int Send(const std::string &str)
        {
            if (str.length() <= 0)
                return 0;
            return Send(str.c_str(), str.length());
        }

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
            if (buffer == NULL || length <= 0)
            {
                SetLastError("Input buffer error!");
                return -1;
            }

            if (msTimeout > 0)
            {
                fd_set fdSet;
                struct timeval tv;

                tv.tv_sec = 0;
                tv.tv_usec = msTimeout * 1000;
                FD_ZERO(&fdSet);
                FD_SET(m_socket, &fdSet);
                int check = select(m_socket + 1, &fdSet, NULL, NULL, &tv);
                if (check == 0)
                {
                    SetLastError("Receive time out!");
                    return 0;
                }
                else if (check < 0)
                {
                    SetLastError("Receive select failed!");
                    return -1;
                }
            }

            //<0 出错 =0 连接关闭 >0 接收到数据大小
            int result = recv(m_socket, buffer, length, 0);
            if (result <= 0)
            {
                SetLastError("Receive failed! Len:" + std::to_string(result) + '.');
                return -1;
            }
            return result;
        }

        std::string Receive(int maxBufferLength, int msTimeout = 0, int* receiveLen = NULL)
        {
            if (maxBufferLength <= 0)
                return "";

            //缓存
            std::shared_ptr<char> buffer(new char[maxBufferLength], std::default_delete<char[]>());
            char *pbuffer = buffer.get();

            int len = Receive(pbuffer, maxBufferLength, msTimeout);
            if (receiveLen)
                *receiveLen = len;
            if (len <= 0)
                return "";
            std::string ret = std::string(pbuffer);
            return ret;
        }

    public:
        std::string GetLastErrorMessage(int* errorNum = NULL)
        {
            return ((SocketHelper *)this)->GetLastErrorMessage(errorNum);
        }

    private:
        bool m_isConnected;
        int m_clientPort;
        RWLockHelper m_rwMutex; //读写锁
    };

} // namespace aigc