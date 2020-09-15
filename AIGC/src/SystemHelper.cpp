#include "SystemHelper.h"

#ifdef _WIN32
    #include "windows.h"
#endif

namespace aigc
{
    bool SystemHelper::IsWindows()
    {
        std::string name = GetSystemName();
        if (name.find("Windows") >= 0)
            return true;
        return false;
    }

    std::string SystemHelper::GetSystemName()
    {
        std::string os_name = "";
#ifdef _WIN32
        OSVERSIONINFO osver = {sizeof(OSVERSIONINFO)};
        GetVersionEx(&osver);
        if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 0)
            os_name = "Windows 2000";
        else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 1)
            os_name = "Windows XP";
        else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 0)
            os_name = "Windows 2003";
        else if (osver.dwMajorVersion == 5 && osver.dwMinorVersion == 2)
            os_name = "Windows vista";
        else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 1)
            os_name = "Windows 7";
        else if (osver.dwMajorVersion == 6 && osver.dwMinorVersion == 2)
            os_name = "Windows 10";
#endif
        return os_name;
    }
}

int main()
{
    SYSTEM_INFO info;
    GetSystemInfo(&info);
    int tmp = info.dwNumberOfProcessors;

    std::string ret = aigc::SystemHelper::GetSystemName();
    return 0;
}