#pragma once

#include <string>
#include "aigc/common/FileHelper.h"
#include "aigc/common/StringHelper.h"

namespace aigc
{
    #define CHINESE_WORD_FILE "./ChineseWords.data"
    class ChineseHelper
    {
    public:
        /**
         * @brief 是否为纯中文
         */
        static bool IsChinese(const std::string &str)
        {
            unsigned char utf[4] = {0};
            unsigned char unicode[3] = {0};
            for (int i = 0; i < (int)str.length(); i++)
            {
                if ((str[i] & 0x80) == 0)
                    return false;

                utf[0] = str[i];
                utf[1] = str[i + 1];
                utf[2] = str[i + 2];
                i += 2;

                unicode[0] = ((utf[0] & 0x0F) << 4) | ((utf[1] & 0x3C) >> 2);
                unicode[1] = ((utf[1] & 0x03) << 6) | (utf[2] & 0x3F);
                if (unicode[0] >= 0x4e && unicode[0] <= 0x9f)
                {
                    if (unicode[0] == 0x9f && unicode[1] > 0xa5)
                        return false;
                    continue;
                }
                return false;
            }
            return true;
        }

        /**
         * @brief 字符串转为简体中文
         */
        static std::string ToSimplified(const std::string& text)
        {
            return Convert(text, true);
        }

        /**
         * @brief 字符串转为繁体中文
         */
        static std::string ToTraditional(const std::string &text)
        {
            return Convert(text, false);
        }

    private:
        static bool ReadChineseWords(std::string& simplified, std::string& traditional)
        {
            std::string content = FileHelper::Read(CHINESE_WORD_FILE);
            if (content.empty())
                return false;

            simplified = StringHelper::SubString(content, "SIMPLIFIED_WORDS=\"", "\"", true);
            traditional = StringHelper::SubString(content, "TRADITIONAL_WORDS=\"", "\"", true);
            if (simplified.empty() || traditional.empty())
                return false;
        }

        /**
         * @brief 简繁相互转化
         * @note 一个中文占3个char
         */
        static std::string Convert(const std::string& text, bool toSimplified = false)
        {
            static std::string Simplified;
            static std::string Traditional;

            if (Simplified.empty() || Traditional.empty())
            {
                if (!ReadChineseWords(Simplified, Traditional))
                    return "";
            }

            std::string strSource;
            std::string strTarget;
            if (toSimplified)
            {
                strSource = Traditional;
                strTarget = Simplified;
            }
            else
            {
                strSource = Simplified;
                strTarget = Traditional;
            }

            int len = text.length();
            if (len <= 0)
                return text;

            std::string ret = "";
            for (int i = 0; i < len; )
            {
                char word[4];
                word[0] = text[i];
                word[1] = text[i + 1];
                word[2] = text[i + 2];
                word[3] = 0;

                if (!IsChinese(word))
                {
                    ret += text[i];
                    i++;
                    continue;
                }

                int intChar = (int)strSource.find(word);
                if (intChar >= 0)
                {
                    ret += strTarget[intChar];
                    ret += strTarget[intChar + 1];
                    ret += strTarget[intChar + 2];
                }
                i += 3;
            }
            return ret;
        }
    };

} // namespace aigc
