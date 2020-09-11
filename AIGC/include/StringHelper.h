#pragma once
#include <string>
#include <vector>

namespace aigc
{
class StringHelper
{
public:
    /**
     * @brief 是否为空白字符串(只有空格和\n)
     * @param str 输入字符串
     * @return 是/否
     */
    static bool IsEmpty(const std::string &str);

    /**
     * @brief 去除头部字符
     * @param str 输入字符串
     * @param remove 要去除的字符
     */
    static std::string TrimLeft(const std::string &str, const std::string remove = " ");

    /**
     * @brief 去除尾部字符
     * @param str 输入字符串
     * @param remove 要去除的字符
     */
    static std::string TrimRight(const std::string &str, const std::string remove = " ");

    /**
     * @brief 去除头尾
     * @param str 输入字符串
     * @param remove 要去除的字符
     */
    static std::string Trim(const std::string &str, const std::string remove = " ");

    /**
     * @brief 左侧补位
     * @param str 输入字符串
     * @param length 长度
     * @param add 补位字符
     */
    static std::string ShiftLeft(const std::string &str, int length, const char add = '0');

    /**
     * @brief 右侧补位
     * @param str 输入字符串
     * @param length 长度
     * @param add 补位字符
     */
    static std::string ShiftRight(const std::string &str, int length, const char add = '0');

    /**
     * @brief 是否为英文
     * @param str 输入字符串
     * @return 真假
     */
    static bool IsEnglish(const std::string &str);

    /**
     * @brief 是否为中文
     * @param str 输入字符串
     * @return 真假
     */
    static bool IsChinese(const std::string &str);

    /**
     * @brief 是否为数字
     * @param str 输入字符串
     * @return 真假
     */
    static bool IsNumeric(const std::string &str);

    /**
     * @brief 转小写
     * @param str 输入字符串
     */
    static std::string ToLower(const std::string &str);

    /**
     * @brief 转大写
     * @param str 输入字符串
     */
    static std::string ToUpper(const std::string &str);

    /**
     * @brief 字符串是否相同
     * @param str1 输入字符串1
     * @param str2 输入字符串2
     * @param ignoreUpLow 忽略大小写
     * @return 真假
     */
    static bool IsSame(const std::string &str1, const std::string &str2, bool ignoreUpLow = false);

    /**
     * @brief 获取子串
     * @param str 输入字符串
     * @param start 开始标志
     * @param end 结束标志
     * @param needEnd 必须匹配结束标志
     */
    static std::string Substring(const std::string &str, const std::string &start = "", const std::string &end = "", bool needEnd = false);

    /**
     * @brief 分割字符串
     * @param str 输入字符串
     * @param seperator 要分割的字符集合
     * @return 字符串数组
     */
    static std::vector<std::string> Split(const std::string &str, char seperator);

    static std::string Replace(const std::string &str, const std::string &fromStr, const std::string &toStr);
};
}

