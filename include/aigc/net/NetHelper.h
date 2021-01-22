#pragma once

#ifdef _WIN32
#include <winsock2.h>
#else
//#include <netdb.h>
//#include <sys/socket.h>
//#include <sys/time.h>
//#include <sys/epoll.h>
//#include <arpa/inet.h>
//#include <unistd.h>
#endif

#include <vector>
#include <string>
#include <regex>

#include "aigc/common/StringHelper.h"

namespace aigc
{

    class NetHelper
    {

    public:
        /**
         * @brief ��ʼ������ģ�飨windowsר�ã�
         */
        static bool Init()
        {
            static bool isInit = false;
            if (isInit)
                return true;

#ifdef _WIN32
            WSADATA wsaData;
            if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
                return false;

            if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2)
            {
                WSACleanup();
                return false;
            }
#endif
            isInit = true;
            return true;
        }

        /**
         * @brief ����ģ���ͷţ�windowsר�ã�
         */
        static void Uinit()
        {
#ifdef _WIN32
            WSACleanup();
#endif
        }

    public:
        /******************************************************
        *
        * IP/�˿ڲ��� 
        *
        ******************************************************/

        /**
         * @brief ͨ��host��ȡIP����
         * @param host ��ַ
         */
        static strings GetIPList(std::string host)
        {
            strings array;
            if (IsIP(host))
            {
                array.push_back(host);
                return array;
            }

            Init();
            struct hostent* he = gethostbyname(host.c_str());
            if (he == NULL)
                return array;

            // ����ַ����ת���ɳ�����ʽ
            struct in_addr inAddr;
            memmove(&inAddr, he->h_addr, 4);

            for (int i = 0;; i++)
            {
                char* p = he->h_addr_list[i];
                if (NULL == p)
                    break;

#ifdef _WIN32
                memcpy(&(inAddr.S_un.S_addr), p, he->h_length);
#else
                memcpy(&(inAddr.s_addr), p, he->h_length);
#endif
                std::string ip = GetIP(inAddr);
                array.push_back(ip);
            }

            return array;
        }

        /**
         * @brief �Ƿ�ΪIP��ַ
         */
        static bool IsIP(std::string str)
        {
            std::regex e("((25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))\\.){3}(25[0-5]|2[0-4]\\d|((1\\d{2})|([1-9]?\\d)))");
            if (std::regex_match(str, e))
                return true;
            return false;
        }

        /**
         * @brief ͨ��host��ȡIP
         * @param host ��ַ
         * @param index �ڼ���IP
         */
        static std::string GetIP(std::string host, int index = 0)
        {
            strings array = GetIPList(host);
            if ((int)array.size() <= 0)
                return "";
            if ((int)array.size() - 1 < index)
                index = array.size() - 1;
            return array[index];
        }

        /**
         * @brief ͨ��in_addr��ȡIP
         */
        static std::string GetIP(struct in_addr inAddr)
        {
            Init();
            char* ip = inet_ntoa(inAddr);
            return std::string(ip);
        }

        /**
         * @brief ��ȡ����IP����
         */
        static strings GetLocalIP()
        {
            Init();
            char host[256];
            if (gethostname(host, 256) != 0)
                return strings();
            return GetIPList(host);
        }

        /**
         * @brief ͨ��socket��ȡ�˿ں�
         */
        static int GetPortBySocket(int socket)
        {
            Init();
            struct sockaddr_in addr;
            int len = sizeof(addr);

            int port = -1;
            if (getsockname(socket, (struct sockaddr*)&addr, &len) == 0)
                port = ntohs(addr.sin_port);
            return port;
        }

    public:
        /******************************************************
        *
        * URL���� 
        *
        ******************************************************/

		typedef struct UrlInfoStruct
		{
			std::string scheme;  //http �� https
			std::string domain;  //����
			std::string port;    //�˿�
			std::string path;    //·��
		}UrlInfo;

        /**
         * @brief ��ȡ�����������
         * @param url ����
         */
        static UrlInfo GetUrlInfo(const std::string& url)
        {
            UrlInfo ret = { "http", "", "80", url };

			//��ȡ��ʽ
			std::string tmpUrl = StringHelper::Trim(url);
            size_t pos = tmpUrl.find("://");
            if (pos != std::string::npos)
            {
                ret.scheme = tmpUrl.substr(0, pos);
                ret.path = tmpUrl.substr(pos + 3);
            }

            //ȥ��λ��Ƭ��
            pos = ret.path.find('#');
            if (pos != std::string::npos)
                ret.path.resize(pos);

            //��ȡ����
            pos = ret.path.find('/');
            if (pos == std::string::npos)
            {
                ret.domain = ret.path;
                ret.path = "/";
            }
            else
            {
                ret.domain = ret.path.substr(0, pos);
                ret.path = ret.path.substr(pos);
            }

            //��ȡ�˿�
            pos = ret.domain.find(':');
            if (pos != std::string::npos)
            {
                ret.port = ret.domain.substr(pos + 1);
                ret.domain.resize(pos);
            }
            else if (ret.scheme == "https")
                ret.port = "443";
            return ret;
        }

        /**
         * @brief Url����
         * @param str Ҫ������ַ���
         */
        static std::string UrlEncode(const std::string& str)
        {
            std::string result;
            constexpr char hexChars[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };

            for (auto i = str.begin(); i != str.end(); ++i)
            {
                const std::uint8_t cp = *i & 0xFF;

                if ((cp >= 0x30 && cp <= 0x39) ||           // 0-9
                    (cp >= 0x41 && cp <= 0x5A) ||           // A-Z
                    (cp >= 0x61 && cp <= 0x7A) ||           // a-z
                    cp == 0x2D || cp == 0x2E || cp == 0x5F) // - . _
                    result += static_cast<char>(cp);
                else if (cp <= 0x7F) // length = 1
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                else if ((cp >> 5) == 0x06) // length = 2
                {
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                    if (++i == str.end())
                        break;
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                }
                else if ((cp >> 4) == 0x0E) // length = 3
                {
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                    if (++i == str.end())
                        break;
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                    if (++i == str.end())
                        break;
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                }
                else if ((cp >> 3) == 0x1E) // length = 4
                {
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                    if (++i == str.end())
                        break;
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                    if (++i == str.end())
                        break;
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                    if (++i == str.end())
                        break;
                    result += std::string("%") + hexChars[(*i & 0xF0) >> 4] + hexChars[*i & 0x0F];
                }
            }

            return result;
        }
    };


} // namespace aigc