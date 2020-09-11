#include "CRCHelper.h"

namespace aigc
{

/**
 * CRC16 校验字节的生成步骤如下： 
 * 1) CRC16 校验寄存器赋值为 0xFFFF； 
 * 2) 取被校验串的第一个字节赋值给临时寄存器； 
 * 3) 临时寄存器与 CRC16 校验寄存器的高位字节进行“异或”运算，赋值给 CRC16 校验寄存器； 
 * 4) 取 CRC16 校验寄存器最后一位赋值给检测寄存器； 
 * 5) 把 CRC16 校验寄存器右移一位； 
 * 6) 若检测寄存器值为 1，CRC16 校验寄存器与多项式 0xA001 进行“异或”运算，赋值给 CRC16校验寄存器； 
 * 7) 重复步骤 4~6，直至移出 8 位； 
 * 8) 取被校验串的下一个字节赋值给临时寄存器； 
 * 9) 重复步骤 3~8，直至被校验串的所有字节均被校验； 
 * 10) 返回 CRC16 校验寄存器的值。 
 * 校验码按照先高字节后低字节的顺序存放。
 */
unsigned int CRCHelper::GetCRC16(unsigned char* str, int length)
{
    unsigned int i, j, check;
    unsigned int crc_reg = 0xFFFF;
    for (i = 0; i < length; i++)
    {
        crc_reg = (crc_reg >> 8) ^ str[i];
        for (j = 0; j < 8; j++)
        {
            check = crc_reg & 0x0001;
            crc_reg >>= 1;
            if (check == 0x0001)
            {
                crc_reg ^= 0xA001;
            }
        }
    }
    return crc_reg;
}


}