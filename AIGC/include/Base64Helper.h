#pragma once
#include <string>
#include <memory>

namespace aigc
{
    class Base64Helper
    {
    public:
        /**
         * @brief Base64 编码
         */
        static std::string Encode(const std::string &str)
        {
            //计算长度
            int len = str.length() / 3 * 4;
            len += str.length() % 3 > 0 ? 4 : 0;
            len += 1;
            if (len <= 1)
                return "";

            //缓存
            std::shared_ptr<unsigned char> buffer(new unsigned char[len], std::default_delete<unsigned char[]>());
            unsigned char *pbuffer = buffer.get();

            unsigned long size = base64_encode((unsigned char *)str.c_str(), str.length(), pbuffer);
            pbuffer[size] = 0;
            return std::string((char *)pbuffer);
        }

        /**
         * @brief Base64 解码
         */
        static std::string Decode(const std::string &str)
        {
            //计算长度
            int len = str.length() / 4 * 3;
            len += str.length() / 4 > 0 ? 3 : 0;
            len += 1;
            if (len <= 1)
                return "";

            //缓存
            std::shared_ptr<unsigned char> buffer(new unsigned char[len], std::default_delete<unsigned char[]>());
            unsigned char *pbuffer = buffer.get();

            unsigned long size = base64_decode((unsigned char *)str.c_str(), str.length(), pbuffer);
            pbuffer[size] = 0;
            return std::string((char *)pbuffer);
        }

    private:
        /******************************************************
         *      
         *  DEFINE定义
         * 
         ******************************************************/
#define AIGC_BASE64_ALPHABET_MAP "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"
#define AIGC_BASE64_REVERSE_MAP                                                             \
    {                                                                                       \
        255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255,     \
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, \
            255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 62, 255, 255, 255, 63,   \
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 255, 255, 255, 255, 255, 255,           \
            255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14,                          \
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 255, 255, 255, 255, 255,            \
            255, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,                \
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 255, 255, 255, 255, 255             \
    }

    private:
        static unsigned long base64_encode(const unsigned char *text,
                                           unsigned long text_len,
                                           unsigned char *encode)
        {
            unsigned long i, j;
            std::string alphabet_map = AIGC_BASE64_ALPHABET_MAP;
            for (i = 0, j = 0; i + 3 <= text_len; i += 3)
            {
                //取出第一个字符的前6位并找出对应的结果字符
                //将第一个字符的后2位与第二个字符的前4位进行组合并找到对应的结果字符
                //将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
                //取出第三个字符的后6位并找出结果字符
                encode[j++] = alphabet_map[text[i] >> 2];
                encode[j++] = alphabet_map[((text[i] << 4) & 0x30) | (text[i + 1] >> 4)];
                encode[j++] = alphabet_map[((text[i + 1] << 2) & 0x3c) | (text[i + 2] >> 6)];
                encode[j++] = alphabet_map[text[i + 2] & 0x3f];
            }

            if (i < text_len)
            {
                unsigned long tail = text_len - i;
                if (tail == 1)
                {
                    encode[j++] = alphabet_map[text[i] >> 2];
                    encode[j++] = alphabet_map[(text[i] << 4) & 0x30];
                    encode[j++] = '=';
                    encode[j++] = '=';
                }
                else //tail==2
                {
                    encode[j++] = alphabet_map[text[i] >> 2];
                    encode[j++] = alphabet_map[((text[i] << 4) & 0x30) | (text[i + 1] >> 4)];
                    encode[j++] = alphabet_map[(text[i + 1] << 2) & 0x3c];
                    encode[j++] = '=';
                }
            }
            return j;
        }

        static unsigned long base64_decode(const unsigned char *code,
                                           unsigned long code_len,
                                           unsigned char *plain)
        {
            static unsigned char reverse_map[] = AIGC_BASE64_REVERSE_MAP;
            unsigned long i, j = 0;
            unsigned char quad[4];
            for (i = 0; i < code_len; i += 4)
            {
                //分组，每组四个分别依次转换为base64表内的十进制数
                for (unsigned long k = 0; k < 4; k++)
                    quad[k] = reverse_map[code[i + k]];

                //取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的前2位进行组合
                plain[j++] = (quad[0] << 2) | (quad[1] >> 4);
                if (quad[2] >= 64)
                    break;
                //取出第二个字符对应base64表的十进制数的后4位与第三个字符对应base64表的十进制数的前4位进行组合
                else if (quad[3] >= 64)
                {
                    plain[j++] = (quad[1] << 4) | (quad[2] >> 2);
                    break;
                }
                //取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
                else
                {
                    plain[j++] = (quad[1] << 4) | (quad[2] >> 2);
                    plain[j++] = (quad[2] << 6) | quad[3];
                }
            }
            return j;
        }
    };

} // namespace aigc