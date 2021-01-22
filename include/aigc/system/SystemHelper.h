#pragma once

#include <string>
#include <vector>

#ifdef _WIN32
#include "windows.h"
#endif

namespace aigc
{

    class SystemHelper
    {
    public:
        /**
         * @brief 是否为Windows
         */
        static bool IsWindows()
        {
    #ifdef _WIN32
            return true;
    #endif
            return false;
        }

        /**
         * @brief 是否为Linux
         */
        static bool IsLinux();

        /**
         * @brief 是否为Macos
         */
        static bool IsMacOS();

        /**
         * @brief 获取系统名称
         * @return eg. Windows10/Macos CentOs/Linux Ubuntu
         */
        static std::string GetSystemName()
        {
            std::string os_name = "";
    #ifdef _WIN32
            //OSVERSIONINFO os_ver = {sizeof(OSVERSIONINFO)};
            //GetVersionEx(&os_ver);
            //if (os_ver.dwMajorVersion == 5 && os_ver.dwMinorVersion == 0)
            //    os_name = "Windows 2000";
            //else if (os_ver.dwMajorVersion == 5 && os_ver.dwMinorVersion == 1)
            //    os_name = "Windows XP";
            //else if (os_ver.dwMajorVersion == 6 && os_ver.dwMinorVersion == 0)
            //    os_name = "Windows 2003";
            //else if (os_ver.dwMajorVersion == 5 && os_ver.dwMinorVersion == 2)
            //    os_name = "Windows vista";
            //else if (os_ver.dwMajorVersion == 6 && os_ver.dwMinorVersion == 1)
            //    os_name = "Windows 7";
            //else if (os_ver.dwMajorVersion == 6 && os_ver.dwMinorVersion == 2)
            //    os_name = "Windows 10";
    #endif
            return os_name;
        }

        /**
         * @brief 获取系统版本号
         */
        static std::string GetSystemVersion();

        /**
         * @brief 获取进程ID
         * @param name 进程名
         * @return ID数组
         */
        static std::vector<int> GetProcessId(const std::string& name = "");

        /**
         * @brief 进程是否存在
         * @param name 进程名
         */
        static bool IsProcessExist(const std::string& name = "");

        /**
         * @brief 进程退出
         * @param name 进程名
         */
        static bool KillProcess(const std::string& name = "");

        /**
         * @brief 进程退出
         * @param processId 进程ID
         */
        static bool KillProcess(int processId);

        /**
         * @brief 打开端口
         * @param portId 端口ID
         */
        static bool OpenPort(int portId);
    };

}
