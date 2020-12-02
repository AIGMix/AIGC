#pragma once
#include <string>
#include <vector>
namespace aigc
{
    using strings = std::vector<std::string>;

    class StringHelper
    {
    public:
        /******************************************************
         *      
         *  判断特定字符串
         * 
         ******************************************************/

        /**
         * @brief 是否为空白字符串(只有空格和\n)
         */
        static bool IsEmpty(const std::string &str)
        {
            if (str.length() <= 0)
                return true;
            for (auto var : str)
            {
                if (var != ' ' || var != '\n' || var != '\r')
                    return false;
            }
            return true;
        }

        /**
         * @brief 字符串是否相同
         * @param ignoreUpLow 忽略大小写
         */
        static bool IsSame(const std::string &str1, const std::string &str2, bool ignoreUpLow = false)
        {
            if (!ignoreUpLow)
                return str1 == str2 ? true : false;
            return ToLower(str1) == ToLower(str2) ? true : false;
        }

        /**
         * @brief 是否包含子串
         */
        static bool IsInclude(const std::string &str, const std::string &substr)
        {
            return str.find(substr) >= 0;
        }

        /**
         * @brief 是否包含字符
         */
        static bool IsInclude(const std::string &str, char subchar)
        {
            return str.find(subchar) >= 0;
        }

        /**
         * @brief 是否为英文
         */
        static bool IsEnglish(const std::string &str)
        {
            if (str.length() <= 0)
                return true;
            for (auto var : str)
            {
                if ((int)var >= 65 && (int)var <= 90)
                    continue;
                if ((int)var >= 97 && (int)var <= 122)
                    continue;
                return false;
            }
            return true;
        }

        /**
         * @brief 是否为数字
         */
        static bool IsNumeric(const std::string &str)
        {
            if (str.length() <= 0)
                return true;
            for (auto var : str)
            {
                if ((int)var < 48 || (int)var > 57)
                    return false;
            }
            return true;
        }

        /**
         * @brief 是否为IP
         * @param str 输入字符串
         */
        static bool IsIP(const std::string &str)
        {
            if (str.length() <= 0)
                return false;
            strings lines = Split(Trim(str), '.');
            if (lines.size() != 4)
                return false;

            for (auto var : lines)
            {
                for (auto cvar : var)
                {
                    if (!IsInclude("0123456789", cvar))
                        return false;
                }
                int temp = atoi(var.c_str());
                if (temp < 0 || temp > 255)
                    return false;
            }
            return true;
        }

        /**
         * @brief 是否为邮箱
         * @param str 输入字符串
         */
        static bool IsEmail(const std::string &str);

        /**
         * @brief 是否为手机号
         * @param str 输入字符串
         */
        static bool IsPhoneNumber(const std::string &str);

        /**
         * @brief 是否为链接
         * @param str 输入字符串
         */
        static bool IsUrl(const std::string &str);

        /**
         * @brief 是否为身份证号码
         * @param str 输入字符串
         */
        static bool IsIdentifyCard(const std::string &str);

    public:
        /******************************************************
         *      
         *  转换
         * 
         ******************************************************/

        /**
         * @brief 字符 --> 二进制数字符串
         */
        static std::string ToBinaryString(char ch)
        {
            std::string ret = "";
            unsigned char k = 0x80;
            //1000 000 每次向右移动一位，每次取ch一位，从高到低
            for (int i = 0; i < 8; i++, k >>= 1)
            {
                if (ch & k)
                    ret = ret + '1';
                else
                    ret = ret + '0';
            }
            return ret;
        }

        /**
         * @brief 字符串 --> 二进制数字符串
         */
        static std::string ToBinaryString(const std::string &str)
        {
            std::string ret = "";
            for (int i = 0; i < str.length(); i++)
            {
                char ch = str[i];
                ret += ToBinaryString(ch);
            }
            return ret;
        }

    public: 
        /******************************************************
         *      
         *  字符串格式化
         * 
         ******************************************************/

        /**
         * @brief 去除头部字符
         * @param str 输入字符串
         * @param remove 要去除的字符
         */
        static std::string TrimLeft(const std::string &str, const std::string remove = " ")
        {
            std::string newStr = str;
            if (newStr.length() <= 0 || remove.length() <= 0)
                return newStr;
            return newStr.erase(0, newStr.find_first_not_of(remove));
        }

        /**
         * @brief 去除尾部字符
         * @param str 输入字符串
         * @param remove 要去除的字符
         */
        static std::string TrimRight(const std::string &str, const std::string remove = " ")
        {
            std::string newStr = str;
            if (newStr.length() <= 0 || remove.length() <= 0)
                return newStr;
            return newStr.erase(newStr.find_last_not_of(remove) + 1);
        }

        /**
         * @brief 去除头尾
         * @param str 输入字符串
         * @param remove 要去除的字符
         */
        static std::string Trim(const std::string &str, const std::string remove = " ")
        {
            std::string newStr = str;
            newStr = TrimLeft(newStr, remove);
            newStr = TrimRight(newStr, remove);
            return newStr;
        }

        /**
         * @brief 左侧补位
         * @param str 输入字符串
         * @param length 长度
         * @param add 补位字符
         */
        static std::string ShiftLeft(const std::string &str, int length, const std::string &add = "0")
        {
            int len = length - str.length();
            if (len <= 0)
                return str;
            return Copy(add, len) + str;
        }

        /**
         * @brief 右侧补位
         * @param str 输入字符串
         * @param length 长度
         * @param add 补位字符
         */
        static std::string ShiftRight(const std::string &str, int length, const std::string &add = "0")
        {
            int len = length - str.length();
            if (len <= 0)
                return str;
            return str + Copy(add, len);
        }

        /**
         * @brief 转小写
         * @param str 输入字符串
         */
        static std::string ToLower(const std::string &str)
        {
            std::string newStr = str;
            for (int i = 0; i < (int)newStr.length(); i++)
            {
                //A-Z 65-90 a-z 97-122
                if ((int)newStr[i] >= 65 && (int)newStr[i] <= 90)
                    newStr[i] = 97 + newStr[i] - 65;
            }
            return newStr;
        }

        /**
         * @brief 转大写
         * @param str 输入字符串
         */
        static std::string ToUpper(const std::string &str)
        {
            std::string newStr = str;
            for (int i = 0; i < (int)newStr.length(); i++)
            {
                //A-Z 65-90 a-z 97-122
                if ((int)newStr[i] >= 97 && (int)newStr[i] <= 122)
                    newStr[i] = 65 + newStr[i] - 97;
            }
            return newStr;
        }

        /**
         * @brief 复制
         * @param str 输入字符串
         * @param count 复制的次数
         */
        static std::string Copy(const std::string &str, int count)
        {
            std::string newStr = str;
            for (int i = 0; i < count; i++)
                newStr += str;
            return newStr;
        }

    public:
        /******************************************************
         *      
         *  字符串处理
         * 
         ******************************************************/

        /**
         * @brief 获取子串
         * @param str 输入字符串
         * @param start 开始标志
         * @param end 结束标志
         * @param needEnd 必须匹配结束标志
         */
        static std::string SubString(const std::string &str,
                                     const std::string &start = "",
                                     const std::string &end = "",
                                     bool needEnd = false)
        {
            if (str.length() <= 0)
                return "";
            if (start.length() <= 0 && end.length() <= 0)
                return "";

            int iStartIdx = start.length() > 0 ? str.find(start) : 0;
            if (iStartIdx < 0)
                return "";
            int iEndIdx = end.length() > 0 ? str.find(end, iStartIdx) : 0;

            //只需匹配结尾
            if (start.length() <= 0)
            {
                if (iEndIdx < 0)
                    return "";
                return str.substr(0, iEndIdx);
            }
            //只匹配开头
            if (end.length() <= 0)
            {
                return str.substr(iStartIdx + start.length());
            }

            //硬性要求前后都匹配
            if (needEnd && iEndIdx < 0)
                return "";
            if (iEndIdx >= 0)
                return str.substr(iStartIdx + start.length(), iEndIdx - iStartIdx - start.length());
            else
                return str.substr(iStartIdx + start.length());
        }

        /**
         * @brief 分割字符串
         * @param str 输入字符串
         * @param seperator 要分割的字符集合
         * @return 字符串数组
         */
        static strings Split(const std::string &str, char seperator)
        {
            strings ret;
            auto string_find_first_not = [str, seperator](size_t pos = 0) -> size_t {
                for (size_t i = pos; i < str.size(); i++)
                {
                    if (str[i] != seperator)
                        return i;
                }
                return std::string::npos;
            };

            size_t lastPos = string_find_first_not(0);
            size_t pos = str.find(seperator, lastPos);
            while (lastPos != std::string::npos)
            {
                ret.emplace_back(str.substr(lastPos, pos - lastPos));
                lastPos = string_find_first_not(pos);
                pos = str.find(seperator, lastPos);
            }

            return ret;
        }

        static strings Split(const std::string &str, std::vector<char> seperators)
        {
            std::vector<std::string> ret;
            if (str.empty())
                return ret;
            if (seperators.size() <= 0)
            {
                ret.push_back(str);
                return ret;
            }

            char sep = seperators[0];
            std::vector<std::string> array = Split(str, sep);
            if (seperators.size() <= 1)
            {
                ret = array;
                return ret;
            }

            seperators.erase(seperators.begin());
            for (int i = 0; i < (int)array.size(); i++)
            {
                std::vector<std::string> subArray = Split(array[i], seperators);
                for (int j = 0; j < (int)subArray.size(); j++)
                {
                    ret.push_back(subArray[j]);
                }
            }
            return ret;
        }

        /**
         * @brief 替换子串
         * @param str 输入字符串
         * @param fromStr 要替换的子串
         * @param toStr 新的子串
         */
        static std::string Replace(const std::string &str,
                                   const std::string &fromStr,
                                   const std::string &toStr)
        {
            std::string ret = str;
            std::string::size_type pos = 0;
            std::string::size_type srclen = fromStr.size();
            std::string::size_type dstlen = toStr.size();

            while ((pos = ret.find(fromStr, pos)) != std::string::npos)
            {
                ret = ret.replace(pos, srclen, toStr);
                pos += dstlen;
            }

            return ret;
        }
    };
} // namespace aigc
