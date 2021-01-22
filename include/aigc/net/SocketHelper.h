#pragma once

#ifdef _WIN32
#define AIGC_SOCKET_ERROR WSAGetLastError()
#else
#include <netdb.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/epoll.h>
#include <arpa/inet.h>
#include <unistd.h>
#define AIGC_SOCKET_ERROR errno
#define INVALID_SOCKET -1
#endif

#include "aigc/net/NetHelper.h"

namespace aigc
{

    class SocketHelper
    {
    public:

        SocketHelper()
        {
            NetHelper::Init();

            m_socket = (int)INVALID_SOCKET;
            m_port = 0;
            m_ip = "";

            m_lastError = 0;
            m_lastErrorMessage = "";
        }

        ~SocketHelper()
        {
            SocketClose(m_socket);
        }

    public:
        /**
         * @brief 创建套件字
         * @param host 地址
         * @param port 端口号
         */
        bool SocketCreat(const std::string& host, int port)
        {
            m_ip = NetHelper::GetIP(host);
            if (m_ip.empty())
            {
                SetLastError("Can't parse host!");
                return false;
            }

            m_port = port;
            m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
            if (m_socket == INVALID_SOCKET)
            {
                SetLastError("Create socket failed!");
                return false;
            }
            return true;
        }

        /**
         * @brief 关闭套接字
         * @param socket 套接字
         */
        void SocketClose(int& socket)
        {
            if (socket != (int)INVALID_SOCKET)
            {
#ifdef _WIN32
                closesocket(socket);
#else
                close(socket);
#endif
            }
            socket = (int)INVALID_SOCKET;
        }


    public:

        /**
         * @brief 发送数据
         * @param socket 套接字
         * @param buffer 缓存
         * @param length 缓存长度
         * @return 发送的长度
         */
        int SocketSend(int socket, const char* buffer, int length)
        {
            if (buffer == NULL || length <= 0)
                return 0;

            int result = send(m_socket, buffer, length, 0);
            if (result <= 0)
            {
                SetLastError("Send failed!");
                return result;
            }
            return result;
        }

        /**
         * @brief 接收数据
		 * @param socket 套接字
		 * @param buffer 数据缓存
         * @param length 缓存大小
         * @return 接收到的数据长度
         */
        int SocketReceive(int socket, char* buffer, size_t length)
        {
            if (buffer == NULL || length <= 0)
                return 0;

            //<0 出错 =0 连接关闭 >0 接收到数据大小
            int result = recv(socket, buffer, length, 0);
            if (result <= 0)
            {
                SetLastError("Receive failed! Len:" + std::to_string(result) + '.');
                return result;
            }
            return result;
        }

    public:
        /**
         * @brief 多路复用
         * @param fdSet 
         * @param sockets 套接字集合
         * @param msTimeout 超时时间（小于等于0表示一直等待）
         */
        bool SocketSelect(fd_set& fdSet, std::vector<int> sockets, int msTimeout)
        {
            //添加套接字
			int maxId = -1;
			FD_ZERO(&fdSet);
			for (auto var : sockets)
			{
				FD_SET(var, &fdSet);
				if (var > maxId)
					maxId = var;
			}

            if (maxId < 0)
                return false;

			//设置等待时间
			struct timeval tv;
            if (msTimeout > 0)
            {
                tv.tv_sec = msTimeout / 1000;
                tv.tv_usec = (msTimeout - tv.tv_sec * 1000) * 1000;
            }

            //开始等待
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
            return true;
        }


    public:
        /**
         * @brief 获取错误信息
         * @param errorMessage 错误信息
         * @return 错误码
         */
        int GetLastError(std::string& errorMessage)
        {
            errorMessage = m_lastErrorMessage;
            return m_lastError;
        }

        /**
         * @brief 获取错误信息
         * @param errorNum 错误码
         * @return 错误信息
         */
        std::string GetLastErrorMessage(int* errorNum = NULL)
        {
            if (errorNum)
                *errorNum = m_lastError;
            return m_lastErrorMessage;
        }

        /**
         * @brief 设置错误信息
		 * @param errorMessage 错误信息
		 * @param appendError 是否在后部添加错误码
         */
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
        int m_socket;           //套接字
        int m_port;             //端口号
        std::string m_ip;       //IP地址

    private:
        int m_lastError;                //最后的错误
        std::string m_lastErrorMessage; //最后的错误信息
    };


} // namespace aigc