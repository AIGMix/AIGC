#pragma once

#include <string>
#include <vector>

namespace aigc
{

class SystemHelper
{
public:
    static bool IsWindows();
    static bool IsLinux();
    static bool IsMacOS();
    static std::string GetSystemName();
    static std::string GetSystemVersion();

    static int GetProcessId(const std::string& name = "");
    static bool KillProcess(const std::string& name = "");

    static bool OpenPort(int portId);
};

}
