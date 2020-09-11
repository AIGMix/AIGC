#include "SocketHelper.h"
#include <memory>

#ifdef _WIN32
    #include <winsock2.h>
    #include <errno.h>
    #include <ws2tcpip.h>
    #define CloseSocket(s) closesocket(s)
    #define MSG_NOSIGNAL 0
    #define ERROR WSAGetLastError()
    #define EINTR WSAEINTR
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
    #include <arpa/inet.h>
    #include <unistd.h>
    #define CloseSocket(s) close(s)
    #define ERROR errno
    static SOCKET_AIGC s_invalidSocket = -1;
    bool Startup(){return true;}
    void CleanStartup(){}
#endif

namespace aigc
{

static bool g_haveStartup = false;

SocketHelper::SocketHelper(SocketHelper::Protocol protocol)
{
    m_protocal = protocol;
    m_socket = s_invalidSocket;
    m_lastError = 0;
    m_lastErrorMessage = "";

    if (g_haveStartup == false)
    {
        if (Startup())
            g_haveStartup = true;
    }
}

SocketHelper::~SocketHelper()
{
    if (m_socket != s_invalidSocket)
        CloseSocket(m_socket);
    m_socket = s_invalidSocket;
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
    while (check == -1 && ERROR == EINTR)
    {
        check = connect(m_socket, address, addressSize);
    }

    if (check == -1)
    {
        m_lastError = ERROR;
        m_lastErrorMessage = "Failed to connect!";
        return false;
    }
    return true;
}

bool SocketHelper::Connect(std::string ip, int port)
{
    //连接
    struct sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(ip.c_str());
    serverAddress.sin_port = htons(port);

    return Connect((struct sockaddr *)&serverAddress, sizeof(serverAddress));
}

bool SocketHelper::Connect(std::string domain, std::string port)
{
    addrinfo hints = {};
    hints.ai_family = GetAddressFamily(m_protocal);
    hints.ai_socktype = SOCK_STREAM;

    addrinfo *info;
    if (getaddrinfo(domain.c_str(), port.c_str(), &hints, &info) != 0)
    {
        m_lastError = ERROR;
        m_lastErrorMessage = "Failed to get address info of " + domain;
        return false;
    }
    std::unique_ptr<addrinfo, decltype(&freeaddrinfo)> addressInfo(info, freeaddrinfo);

    return Connect(addressInfo->ai_addr, (int)addressInfo->ai_addrlen);
}

int SocketHelper::Send(const char *buffer, int length, int flags)
{
    if (m_socket == s_invalidSocket)
        return -1;

    int result = send(m_socket, buffer, length, flags);
    while (result == -1 && ERROR == EINTR)
    {
        result = send(m_socket, buffer, length, flags);
    }

    if (result == -1)
    {
        m_lastError = ERROR;
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
    while (result == -1 && ERROR == EINTR)
    {
        result = send(m_socket, buffer, length, flags);
    }

    if (result == -1)
    {
        m_lastError = ERROR;
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