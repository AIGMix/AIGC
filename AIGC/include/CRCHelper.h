#pragma once

#include <string>

namespace aigc
{

class CRCHelper
{
public:
    /**
     * @brief 获取CRC16检验码
     * @param str 需要检验的数据串
     * @param length 数据串长度
     */
    unsigned int GetCRC16(unsigned char* str, int length);

};

}