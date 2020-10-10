#pragma once

#include <string>

namespace aigc
{

class CRCHelper
{
public:
    enum class Type 
    {
        CRC16,
        CRC32,
    };

    /**
     * @brief 获取CRC校验码
     * @param type 类型
     * @param str 需要检验的数据串
     * @param length 数据串长度
     */
    static unsigned int Get(Type type, unsigned char *str, int length);

    /**
     * @brief 获取文件CRC校验码
     * @param type 类型
     * @param filePath 文件路径
     */
    static unsigned int GetFile(Type type, std::string filePath);
};



}