#pragma once

#include <string>
#include <vector>

namespace aigc
{

class SystemHelper
{
public:
    /**
     * @brief 是否为Windows
     */
    static bool IsWindows();

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
     * @return eg. Windows10/Macos Centos/Linux Ubuntu
     */
    static std::string GetSystemName();

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
