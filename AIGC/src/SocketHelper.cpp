#include "SocketHelper.h"
#include "StringHelper.h"
#include <memory>

#ifdef _WIN32
    #include <winsock2.h>
    #include <errno.h>
    #include <ws2tcpip.h>
    #define CloseSocket(s) closesocket(s)
    #define MSG_NOSIGNAL 0
    #define AIG_SOCKET_ERROR WSAGetLastError()
    #define SOCKET_EINTR WSAEINTR
    static SOCKET_AIGC s_invalidSocket = INVALID_SOCKET;
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
#else
    #include <sys/socket.h>
    #include <sys/time.h>
    #include <sys/epoll.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define CloseSocket(s) close(s)
    #define AIG_SOCKET_ERROR errno
    static SOCKET_AIGC s_invalidSocket = -1;
    bool Startup(){return true;}
    void CleanStartup(){}
#endif



namespace aigc
{


SocketHelper::SocketHelper(SocketHelper::Protocol protocol)
{
    m_protocal = protocol;
    m_socket = s_invalidSocket;
    m_lastError = 0;
    m_lastErrorMessage = "";

    Startup();
}

SocketHelper::~SocketHelper()
{
    if (m_socket != s_invalidSocket)
        CloseSocket(m_socket);
    m_socket = s_invalidSocket;

    CleanStartup();
}

int SocketHelper::GetAddressFamily(Protocol protocol)
{
    if (protocol == Protocol::IPv4)
        return AF_INET;
    else
        return AF_INET6;
}

void SocketHelper::CloseConnect()
{
    if (m_socket != s_invalidSocket)
    {    
        CloseSocket(m_socket);
        m_socket = s_invalidSocket;
    }
}

bool SocketHelper::Connect(const struct sockaddr *address, int addressSize)
{
    if (m_socket != s_invalidSocket)
        CloseConnect();

    //创建socket
    m_socket = socket(GetAddressFamily(m_protocal), SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == s_invalidSocket)
        return false;

    int check = connect(m_socket, address, addressSize);
    while (check == -1 && AIG_SOCKET_ERROR == SOCKET_EINTR)
    {
        check = connect(m_socket, address, addressSize);
    }

    if (check == -1)
    {
        m_lastError = AIG_SOCKET_ERROR;
        m_lastErrorMessage = "Failed to connect!";
        return false;
    }
    return true;
}

bool SocketHelper::Connect(std::string host, int port)
{
    if (StringHelper::IsIP(host))
    {
        std::string ip = host;
        struct sockaddr_in serverAddress;
        serverAddress.sin_family = AF_INET;
        serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
        serverAddress.sin_port = htons(port);
        return Connect((struct sockaddr *)&serverAddress, sizeof(serverAddress));
    }
    else 
    {
        std::string domain = host;
        addrinfo hints = {};
        hints.ai_family = GetAddressFamily(m_protocal);
        hints.ai_socktype = SOCK_STREAM;

        addrinfo *info;
        if (getaddrinfo(domain.c_str(), std::to_string(port).c_str(), &hints, &info) != 0)
        {
            m_lastError = AIG_SOCKET_ERROR;
            m_lastErrorMessage = "Failed to get address info of " + domain;
            return false;
        }
        std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> addressInfo(info, freeaddrinfo);

        return Connect(addressInfo->ai_addr, (int)addressInfo->ai_addrlen);
    }
}

bool SocketHelper::Bind(int port, std::string host)
{
    if (m_socket == s_invalidSocket)
        return -1;

    sockaddr_in saddr;
    saddr.sin_family = GetAddressFamily(m_protocal);
    saddr.sin_port = htons(port);
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (host.length() > 0)
    {
        struct in_addr addr;
        struct hostent *he = gethostbyname(host.c_str());
        if (he && he->h_addrtype == AF_INET)
            saddr.sin_addr = *reinterpret_cast<struct in_addr *>(he->h_addr);
        else
        {
            m_lastError = AIG_SOCKET_ERROR;
            m_lastErrorMessage = "Bind port " + std::to_string(port) + " failed in 'gethostbyname'!";
            return false;
        }
    }

    if (::bind(m_socket, (sockaddr *)&saddr, sizeof(saddr)) != 0)
    {
        m_lastError = AIG_SOCKET_ERROR;
        m_lastErrorMessage = "Bind port " + std::to_string(port) + " failed!";
        return false;
    }

    listen(m_socket, 20);
    return true;
}

bool SocketHelper::CreatEpoll()
{
    return true;
}

int SocketHelper::Send(const char *buffer, int length, int flags)
{
    if (m_socket == s_invalidSocket)
        return -1;

    int result = send(m_socket, buffer, length, flags);
    while (result == -1 && AIG_SOCKET_ERROR == SOCKET_EINTR)
    {
        result = send(m_socket, buffer, length, flags);
    }

    if (result == -1)
    {
        m_lastError = AIG_SOCKET_ERROR;
        m_lastErrorMessage = "Failed to send!";
        return false;
    }
    return result;
}

int SocketHelper::Receive(char *buffer, size_t length, int flags)
{
    if (m_socket == s_invalidSocket)
        return -1;

    int result = recv(m_socket, buffer, length, flags);
    while (result == -1 && AIG_SOCKET_ERROR == SOCKET_EINTR)
    {
        result = send(m_socket, buffer, length, flags);
    }

    if (result == -1)
    {
        m_lastError = AIG_SOCKET_ERROR;
        m_lastErrorMessage = "Failed to receive!";
        return false;
    }
    return result;
}

int SocketHelper::GetLastError(std::string &errorMessage)
{
    errorMessage = m_lastErrorMessage;
    return m_lastError;
}

}