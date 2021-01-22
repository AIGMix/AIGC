#pragma once

#include <map>
#include <vector>
#include <string>

namespace aigc
{
    class EmailHelper
    {

    public:
        bool Send(std::string toEmail, std::string title, std::string content);    

    private:
        std::string m_username;
        std::string m_password;

        std::string m_smtpServer;
        int m_smtpPort;
        
        bool m_enableSSL;
    };
}