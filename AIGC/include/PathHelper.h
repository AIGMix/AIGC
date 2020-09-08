#pragma once

#include <string>
#include <vector>

namespace aigc
{

class PathHelper
{
public:
    /**
     * @brief 获取目录大小
     * @param path 路径
     */
    static long GetSize(const std::string& path);

    /**
     * @brief 是否为绝对路径
     * @param path 路径
     */
    static bool IsAbsolutePath(const std::string &path);

    /**
     * @brief 获取绝对路径
     * @param path 相对路径
     */
    static std::string GetFullPath(const std::string& path);

    /**
     * @brief 移除目录限制字符
     * @param path 路径
     * @param newChar 替换的字符
     * @return 信的字符串
     */
    static std::string ReplaceLimitChar(const std::string &path, const std::string &newChar = " ");

    /**
     * @brief 获取目录下的所有文件名
     * @param path 路径
     * @return 文件名路径数组
     */
    static std::vector<std::string> GetAllFiles(const std::string& path);

    /**
     * @brief 新建多层目录
     * @param path 路径
     * @return True 成功
     */
    static bool Mkdirs(const std::string& path);

    /**
     * @brief 删除文件夹
     * @param path 路径
     * @return True 成功
     */
    static bool Remove(const std::string& path);

    /**
     * @brief 复制目录
     * @param path 路径
     * @param toPath 新路径
     * @return True 成功
     */
    static bool Copy(const std::string &path, const std::string &toPath);

    /**
     * @brief 移动目录
     * @param path 路径
     * @param toPath 新路径
     * @return True 成功
     */
    static bool Move(const std::string &path, const std::string &toPath);

    /**
     * @brief 获取当前工作路径
     * @return 路径
     */
    static std::string GetWorkPath();

    /**
     * @brief 获取路径 eg. /root/home/test.json -> /root/home/
     * @param path 源路径名 
     * @return 路径 
     */
    static std::string GetDirName(const std::string& path);

    /**
     * @brief 获取文件名 eg. /root/home/test.json -> test.json
     * @param path 源路径名 
     * @return 文件名(带扩展名)
     */
    static std::string GetFileName(const std::string& path);

    /**
     * @brief 获取文件名(没带扩展) eg. /root/home/test.json -> test
     * @param path 源路径名 
     * @return 文件名(不带扩展名)
     */
    static std::string GetFileNameWithoutExt(const std::string& path);

    /**
     * @brief 获取扩展名 eg. /root/home/test.json -> json
     * @param path 源路径名 
     * @return 扩展名
     */
    static std::string GetFielExt(const std::string &path);
};

} 
