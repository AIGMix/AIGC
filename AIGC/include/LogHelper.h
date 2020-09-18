#pragma once

#include <vector>
#include <string>
#include <mutex>

namespace aigc
{
class LogHelper
{
public:
    enum class Type
    {
        Info,
        Warn,
        Error,
        Debug,
        Notice,
    };

    /**
     * @brief 构造函数
     * @param filePath 日志文件路径
     * @param fielNum 日志数量
     * @param fileMaxSize 日志最大尺寸（兆）
     */
    LogHelper(std::string filePath, int fielNum = 3, int fileMaxSize = 100);
    ~LogHelper();

    /**
     * @brief 写日志
     * @param type 日志类型
     * @param str 记录
     */
    bool Write(Type type, const std::string &str);

private:
    /**
     * @brief 获取类型字符串
     * @param type 类型
     */
    std::string GetTypeString(Type type);

    /**
     * @brief 打开日志
     * @param path 日志文件路径
     */
    bool Open(std::string path);

    /**
     * @brief 关闭日志
     */
    void Close();

    std::mutex m_mutex;
    FILE* m_fp;
};

} // namespace aigc
