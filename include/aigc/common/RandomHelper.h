#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <ctime>


namespace aigc
{
    class RandomHelper
    {
    public:
        /**
         * @brief 设置种子
         */
        static void Init()
        {
            static bool isInit = false;
            if (!isInit)
            {
                srand((unsigned int)time(0));
                isInit = true;
            }
        }

        /**
         * @brief 获取随机数
         * @param min 最小值
         * @param max 最大值
         */
        static int GetInt(int min, int max)
        {
            Init();
            if (min >= max)
                return min;
            int ret = rand() % (max - min) + min;
            return ret;
        }

        /**
         * @brief 获取随机字符串
         * @param length 字符串长度
         * @param haveNumber 是否包括数字
         * @param haveSymbol 是否包括符号
         * @param uppercase 是否包括大写字母
         * @param lowercase 是否包括小写字母
         */
        static std::string GetString(int length,
                                     bool haveNumber = true,
                                     bool haveSymbol = true,
                                     bool uppercase = true,
                                     bool lowercase = true)
        {
            Init();
            std::string pattern = "";
            if (haveNumber)
                pattern += "0123456789";
            if (haveSymbol)
                pattern += "!@#$%^&*(){}[]<>,./=-+~|;'\"";
            if (uppercase)
                pattern += "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
            if (lowercase)
                pattern += "abcdefghijklmnopqrstuvwxyz";

            int patternLen = pattern.length();
            if (patternLen <= 0 || length <= 0)
                return "";

            std::string ret = "";
            for (int i = 0; i < length; i++)
            {
                int random = GetInt(0, patternLen - 1);
                ret += pattern[random];
            }

            return ret;
        }
    };

} // namespace aigc
