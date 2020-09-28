#include "SocketHelper.h"
#include "StringHelper.h"
#include <memory>

#ifdef _WIN32
    #include <winsock2.h>
    #include <errno.h>
    #include <ws2tcpip.h>
    #define MSG_NOSIGNAL 0
    #define GET_ERROR WSAGetLastError()
    #define SOCKET_EINTR WSAEINTR
    bool Startup()
    {
        WSADATA wsaData;
        int error = WSAStartup(MAKEWORD(2, 2), &wsaData);
        if (error != 0)
            return false;

        if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
        {
            WSACleanup();
            return false;
        }
        return true;
    }
    void CleanStartup()
    {
        WSACleanup();
    }
    void CloseSocket(int& socket)
    {
        if (socket != INVALID_SOCKET)
            closesocket(socket);
        socket = INVALID_SOCKET;
    }
#else
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/epoll.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define GET_ERROR errno
    #define INVALID_SOCKET -1;
    bool Startup(){return true;}
    void CleanStartup(){}
    void CloseSocket(int &socket)
    {
        if (socket != INVALID_SOCKET)
            close(socket);
        socket = INVALID_SOCKET;
    }
#endif



namespace aigc
{

    SocketHelper::SocketHelper(std::string hostOrIP, int port)
    {
        Startup();
        m_socket = INVALID_SOCKET;
        m_port = port;
        if (StringHelper::IsIP(hostOrIP))
            m_ip = hostOrIP;
        else
        {
            m_host = hostOrIP;
            m_ip = GetIPByHost(hostOrIP);
        }

        m_lastError = 0;
        m_lastErrorMessage = "";
    }

    SocketHelper::~SocketHelper()
    {
        CloseSocket(m_socket);
        CleanStartup();
    }

    void SocketHelper::SetIPAndPort(std::string hostOrIP, int port)
    {
        if (StringHelper::IsIP(hostOrIP))
            m_ip = hostOrIP;
        else
        {
            m_host = hostOrIP;
            m_ip = GetIPByHost(hostOrIP);
        }
    }

    void SocketHelper::CloseConnect()
    {
        CloseSocket(m_socket);
    }

    bool SocketHelper::Connect()
    {
        CloseConnect();

        //创建socket
        m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (m_socket == INVALID_SOCKET)
            return false;

        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr(m_ip.c_str());
        serverAddress.sin_port = htons(m_port);

        //连接
        int check = connect(m_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        while (check == -1 && GET_ERROR == SOCKET_EINTR)
        {
            check = connect(m_socket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
        }

        if (check == -1)
        {
            CloseConnect();
            SetLastError("Failed to connect!");
            return false;
        }
        return true;
    }

    bool SocketHelper::Bind()
    {
        if (m_socket == INVALID_SOCKET)
            return -1;

        sockaddr_in saddr;
        saddr.sin_family = AF_INET;
        saddr.sin_port = htons(m_port);
        saddr.sin_addr.s_addr = inet_addr(m_ip.c_str());;

        if (bind(m_socket, (sockaddr *)&saddr, sizeof(saddr)) != 0)
        {
            SetLastError("Bind port " + std::to_string(m_port) + " failed!");
            return false;
        }

        listen(m_socket, 20);
        return true;
    }

    bool SocketHelper::Accept()
    {
        // accept();
        return true;
    }

    int SocketHelper::Send(const char *buffer, int length)
    {
        if (m_socket == INVALID_SOCKET)
            return -1;

        int result = send(m_socket, buffer, length, 0);
        while (result == -1 && GET_ERROR == SOCKET_EINTR)
        {
            result = send(m_socket, buffer, length, 0);
        }

        if (result == -1)
        {
            SetLastError("Failed to send!");
            return false;
        }
        return result;
    }

    int SocketHelper::Receive(char *buffer, size_t length, int mstimeout)
    {
        if (m_socket == INVALID_SOCKET)
            return -1;

        if (mstimeout > 0)
        {
            fd_set sdset;
            struct timeval tv;

            tv.tv_sec = 0;
            tv.tv_usec = mstimeout * 1000;
            FD_ZERO(&sdset);
            FD_SET(m_socket, &sdset);
            if (select(m_socket + 1, &sdset, NULL, NULL, &tv) <= 0)
                return -1;
        }

        //<0 出错 =0 连接关闭 >0 接收到数据大小
        //返回值<0时并且(errno == EINTR || errno == EWOULDBLOCK || errno == EAGAIN)的情况下认为连接是正常的，继续接收
        int result = recv(m_socket, buffer, length, 0);
        if (result == -1)
        {
            SetLastError("Failed to receive!");
            return false;
        }
        return result;
    }

    int SocketHelper::GetLastError(std::string &errorMessage)
    {
        errorMessage = m_lastErrorMessage;
        return m_lastError;
    }

    void SocketHelper::SetLastError(std::string errorMessage)
    {
        m_lastErrorMessage = errorMessage;
        m_lastError = GET_ERROR;
    }

    std::string SocketHelper::GetIPByHost(std::string host)
    {
        struct hostent* he = gethostbyname(host.c_str());
        //获取第一个IP
        // if (he && he->h_addrtype == AF_INET && he->h_addr_list != NULL)
        // {
        //     char** pptr = he->h_addr_list;
        //     char buffer[128];
        //     inet_ntop(he->h_addrtype, *pptr, buffer, sizeof(buffer));
        //     return buffer;
        // }
        return "";
    }
}

// using namespace aigc;
// int main()
// {
//     SocketHelper server("127.0.0.1", 11112);
//     server.Bind();
//     while(true)
//     {

//     }
    
//     return 0;
// }