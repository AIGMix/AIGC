#pragma once

#include <string>
#include <vector>
#include <functional>

#include "SocketHelper.h"

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
            std::unique_lock<std::shared_mutex> lock(m_mutex);

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
            std::unique_lock<std::shared_mutex> lock(m_mutex);
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
            std::shared_lock<std::shared_mutex> lock(m_mutex);
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
        bool Select(int msTimeout, ClinetConnectNotify connectFunc = NULL,
                    ClinetDisConnectNotify disconnectFunc = NULL,
                    ReceiveDataNotify receiveFunc = NULL)
        {
            std::shared_lock<std::shared_mutex> lock(m_mutex);
            if (!m_isListen)
            {
                SetLastError("UnListen!");
                return false;
            }

            //添加监听的套接字集合
            fd_set fdSet;
            FD_ZERO(&fdSet);
            FD_SET(m_socket, &fdSet);

            int maxId = m_socket;
            for (auto var : m_clientInfos)
            {
                FD_SET(var.socket, &fdSet);
                if (var.socket > maxId)
                    maxId = var.socket;
            }

            //开始多路等待
            struct timeval tv;
            tv.tv_sec = 0;
            tv.tv_usec = msTimeout * 1000;
            int check = select(maxId + 1, &fdSet, NULL, NULL, msTimeout > 0 ? &tv : NULL);
            if (check == 0)
            {
                SetLastError("Select time out!");
                return false;
            }
            else if (check < 0)
            {
                SetLastError("Select failed!");
                return false;
            }

            //检查是否有新连接
            ClientInfo cinfo;
            if (FD_ISSET(m_socket, &fdSet) && Accept(cinfo))
            {
                if (connectFunc)
                    connectFunc(cinfo);
            }

            //检查是否有新消息
            for (int i = 0; i < m_clientInfos.size(); i++)
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
            std::shared_lock<std::shared_mutex> lock(m_mutex);
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
            info.ip = GetIpByInAddr(raddr.sin_addr);
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
            std::shared_lock<std::shared_mutex> lock(m_mutex);
            if (!m_isListen)
            {
                SetLastError("UnListen!");
                return false;
            }

            int result = recv(client.socket, buffer, length, 0);
            if (result <= 0)
            {
                SetLastError("Receive failed! Len:" + std::to_string(result) + '.');
                return -1;
            }
            return result;
        }

        /**
         * @brief 发送数据
         */
        int Send(ClientInfo client, const char *buffer, size_t length)
        {
            std::shared_lock<std::shared_mutex> lock(m_mutex);
            if (!m_isListen)
            {
                SetLastError("UnListen!");
                return false;
            }

            int result = send(client.socket, buffer, length, 0);
            if (result <= 0)
            {
                SetLastError("Send failed! Len:" + std::to_string(result) + '.');
                return -1;
            }
            return result;
        }

    public:
        std::string GetLastErrorMessage(int *errorNum = NULL)
        {
            return ((SocketHelper *)this)->GetLastErrorMessage(errorNum);
        }

    private:
        bool m_isListen;
        std::shared_mutex m_mutex; //读写锁
        std::vector<ClientInfo> m_clientInfos;
    };
} // namespace aigc