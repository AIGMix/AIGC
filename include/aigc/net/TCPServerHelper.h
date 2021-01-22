#pragma once

#include <functional>

#include "aigc/net/SocketHelper.h"
#include "aigc/system/RWLockHelper.h"
#include "aigc/common/StringHelper.h"

namespace aigc
{
    class TCPServerHelper : SocketHelper
    {

    public:
        TCPServerHelper(const std::string &host = "", int port = 0)
        {
            m_isListen = false;
            if (host != "")
                Listen(host, port);
        }

        ~TCPServerHelper()
        {
            CloseListen();
        }

    public:
        /**
         * @brief 启动监听
         */
        bool Listen(const std::string &host, int port)
        {
            CloseListen();
            ShareRWLock lock(&m_mutex, true);

            //创建socket
            if (!SocketCreat(host, port))
                return false;

            sockaddr_in saddr;
            saddr.sin_family = AF_INET;
            saddr.sin_addr.s_addr = m_ip.length() > 0 ? inet_addr(m_ip.c_str()) : htonl(INADDR_ANY);
            saddr.sin_port = htons(m_port);

            if (bind(m_socket, (sockaddr *)&saddr, sizeof(saddr)) != 0)
            {
                SocketClose(m_socket);
                SetLastError("Bind port " + std::to_string(m_port) + " failed!");
                return false;
            }

            if (listen(m_socket, 32) != 0)
            {
                SocketClose(m_socket);
                SetLastError("Listen failed!");
                return false;
            }

            m_isListen = true;
            return true;
        }

        void CloseListen()
        {
            ShareRWLock lock(&m_mutex, true);
            for (auto var : m_clientInfos)
            {
                SocketClose(var.socket);
            }
            SocketClose(m_socket);
            m_isListen = false;
            m_clientInfos.clear();
        }

        bool IsListen()
        {
            ShareRWLock lock(&m_mutex);
            return m_isListen;
        }

    public:
        //客户端信息
        typedef struct ClientInfoStruct
        {
            int socket;
            int port;
            std::string ip;
            struct sockaddr_in saddr;
        } ClientInfo;

        //回调接口：客户端连接 | 客户端断开 | 消息接收
        typedef std::function<void(ClientInfo info)> ClinetConnectNotify;
        typedef std::function<void(ClientInfo info, std::string message)> ClinetDisConnectNotify;
        typedef std::function<void(ClientInfo info, char *buffer, int bufferLen)> ReceiveDataNotify;

        /**
         * @brief 多路等待
         * @param msTimeout 超时时间
         * @param connectFunc 客户端连接回调
         * @param disconnectFunc 客户端断开回调
         * @param receiveFunc 消息接收回调
         */
        bool Select(int msTimeout, 
                    ClinetConnectNotify connectFunc = NULL,
                    ClinetDisConnectNotify disconnectFunc = NULL,
                    ReceiveDataNotify receiveFunc = NULL)
        {
            ShareRWLock lock(&m_mutex);
            if (!m_isListen)
            {
                SetLastError("UnListen!");
                return false;
            }

            //添加监听的套接字集合
            std::vector<int> sockets = { m_socket };
			for (auto var : m_clientInfos)
			{
                sockets.push_back(var.socket);
			}

			fd_set fdSet;
            if (!SocketSelect(fdSet, sockets, msTimeout))
                return false;

            //检查是否有新连接
            ClientInfo cinfo;
            if (FD_ISSET(m_socket, &fdSet) && Accept(cinfo))
            {
                if (connectFunc)
                    connectFunc(cinfo);
            }

            //检查是否有新消息
            for (int i = 0; i < (int)m_clientInfos.size(); i++)
            {
                if (!FD_ISSET(m_clientInfos[i].socket, &fdSet))
                    continue;

                char pbuffer[1024];
                int len = Receive(m_clientInfos[i], pbuffer, 1024);
                if (len <= 0)
                {
                    if (disconnectFunc)
                        disconnectFunc(m_clientInfos[i], GetLastErrorMessage());

                    SocketClose(m_clientInfos[i].socket);
                    m_clientInfos.erase(std::begin(m_clientInfos) + i);
                    i--;
                }
                else
                {
                    if (receiveFunc)
                        receiveFunc(m_clientInfos[i], pbuffer, len);
                }
            }

            return true;
        }

    public:
        /**
         * @brief 接收客户端连接
         */
        bool Accept(ClientInfo &info)
        {
            ShareRWLock lock(&m_mutex);
            if (!m_isListen)
            {
                SetLastError("UnListen!");
                return false;
            }

            struct sockaddr_in raddr;
            int len = sizeof(raddr);
            int socket = accept(m_socket, (struct sockaddr *)&raddr, &len);
            if (socket <= 0)
            {
                SetLastError("Accept client failed!");
                return false;
            }

            info.socket = socket;
            info.ip = NetHelper::GetIP(raddr.sin_addr);
            info.port = raddr.sin_port;
            memcpy(&info.saddr, &raddr, len);

            m_clientInfos.push_back(info);
            return true;
        }

        /**
         * @brief 接收客户端数据
         */
        int Receive(ClientInfo client, char *buffer, size_t length)
        {
            ShareRWLock lock(&m_mutex);
            if (!m_isListen)
            {
                SetLastError("UnListen!");
                return false;
            }

            return SocketReceive(client.socket, buffer, length);
        }

        /**
         * @brief 发送数据
         */
        int Send(ClientInfo client, const char *buffer, size_t length)
        {
            ShareRWLock lock(&m_mutex);
            if (!m_isListen)
            {
                SetLastError("UnListen!");
                return false;
            }

            return SocketSend(client.socket, buffer, length);
        }

    public:
        std::string GetLastErrorMessage(int *errorNum = NULL)
        {
            return ((SocketHelper *)this)->GetLastErrorMessage(errorNum);
        }

    private:
        bool m_isListen;
        RWLockHelper m_mutex; //读写锁
        std::vector<ClientInfo> m_clientInfos;
    };
} // namespace aigc