#pragma once
#include <string>
#include <memory>
#include <mutex>
#include <iostream>
#include <shared_mutex>

#include "StringHelper.h"

namespace aigc
{
/******************************************************
 *      
 *  跨平台头文件
 * 
 ******************************************************/
#ifdef _WIN32
    #include <winsock2.h>
    #include <errno.h>
    #include <ws2tcpip.h>
    #define AIGC_SOCKET_ERROR WSAGetLastError()
#else
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/epoll.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define AIGC_SOCKET_ERROR errno
    #define INVALID_SOCKET -1;
#endif

    class SocketHelper
    {
    public:

        SocketHelper()
        {
            m_isStartUp = StartUp();

            m_socket = (int)INVALID_SOCKET;
            m_port = 0;
            m_ip = "";

            m_lastError = 0;
            m_lastErrorMessage = "";
        }

        ~SocketHelper()
        {
            SocketClose(m_socket);
            ClearUp();
        }

    public:
        /******************************************************
         *      
         *  socket库绑定与解绑（windows需要）
         * 
         ******************************************************/
        bool StartUp()
        {
#ifdef _WIN32
            WSADATA wsaData;
            int error = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (error != 0)
            {
                SetLastError("WSAStartup failed!");
                return false;
            }

            if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
            {
                SetLastError("WSAStartup error(version wrong)!");
                WSACleanup();
                return false;
            }
#endif
            return true;
        }

        void ClearUp()
        {
#ifdef _WIN32
            WSACleanup();
#endif
        }

    public:
        /******************************************************
         *      
         *  socket创建与关闭
         * 
         ******************************************************/
        bool SocketCreat(const std::string& host, int port)
        {
            //设置端口和ip
            m_port = port;
            m_ip = StringHelper::IsIP(host) ? host : GetIpByHost(host);

            //创建socket
            m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (m_socket == INVALID_SOCKET)
            {
                SetLastError("Creat socket failed!");
                return false;
            }

            return true;
        }
        
        void SocketClose(int &socket)
        {
            if (socket != (int)INVALID_SOCKET)
                closesocket(socket);
            socket = (int)INVALID_SOCKET;
        }
        

    public:
        /******************************************************
         *      
         *  错误信息保存于读取
         * 
         ******************************************************/

        int GetLastError(std::string& errorMessage)
        {
            errorMessage = m_lastErrorMessage;
            return m_lastError;
        }

        std::string GetLastErrorMessage(int* errorNum = NULL)
        {
            if (errorNum)
                *errorNum = m_lastError;
            return m_lastErrorMessage;
        }

        void SetLastError(std::string errorMessage, bool appendError = true)
        {
            m_lastError = AIGC_SOCKET_ERROR;
            m_lastErrorMessage = errorMessage;
            if (appendError) 
            {
                m_lastErrorMessage += "Error:" + std::to_string(m_lastError);
            }
        }

    public:
        /******************************************************
         *      
         *  工具函数
         * 
         ******************************************************/
        strings GetIpsByHost(std::string host)
        {
            strings array;
            struct hostent *he = gethostbyname(host.c_str());
            if (he == NULL)
                return array;

            // 将地址进行转换成常规形式
            struct in_addr inAddr;
            memmove(&inAddr, he->h_addr, 4);

            for (int i = 0;; i++)
            {
                char *p = he->h_addr_list[i];
                if (NULL == p)
                    break;

                memcpy(&(inAddr.S_un.S_addr), p, he->h_length);
                std::string ip = GetIpByInAddr(inAddr);

                array.push_back(ip);
            }

            return array;
        }

        std::string GetIpByHost(std::string host, int index = 0)
        {
            strings array = GetIpsByHost(host);
            if (array.size() <= 0)
                return "";
            if (array.size() - 1 < index)
                index = array.size() - 1;
            return array[index];
        }

        std::string GetIpByInAddr(struct in_addr inAddr)
        {
            char *ip = inet_ntoa(inAddr);
            return std::string(ip);
        }

        std::string GetLocalIp()
        {
            char host[256];
            if (gethostname(host, 256) != 0)
                return "";
            return GetIpByHost(host);
        }

        int GetSelfPortBySocket(int socket)
        {
            struct sockaddr_in addr;
            int len = sizeof(addr);

            int port = -1;
            if (getsockname(m_socket, (struct sockaddr *)&addr, &len) == 0)
                port = ntohs(addr.sin_port);
            return port;
        }
    
    public:
        int m_socket;           //套接字
        int m_port;             //端口号
        std::string m_ip;       //IP地址


        bool m_isStartUp;               //socket库是否绑定成功
        int m_lastError;                //最后的错误
        std::string m_lastErrorMessage; //最后的错误信息
    };


} // namespace aigc