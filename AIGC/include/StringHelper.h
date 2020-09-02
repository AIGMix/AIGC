#pragma once
#include <string>

namespace aigc
{
class StringHelper
{
public:
    /**
     * @brief 是否为空白字符串(只有空格和\n)
     * @param {string} str 输入字符串
     * @return {bool}
     */
    static bool IsEmpty(const std::string &str);

    /**
     * @brief 去除头部字符
     * @param {string} str 输入字符串
     * @param {string} remove 要去除的字符
     */
    static std::string TrimLeft(const std::string &str, const std::string remove = " ");

    /**
     * @brief 去除尾部字符
     * @param {string} str 输入字符串
     * @param {string} remove 要去除的字符
     */
    static std::string TrimRight(const std::string &str, const std::string remove = " ");

    /**
     * @brief 去除头尾
     * @param {string} str 输入字符串
     * @param {string} remove 要去除的字符
     */
    static std::string Trim(const std::string &str, const std::string remove = " ");

    /**
     * @brief 左侧补位
     * @param {string} str 输入字符串
     * @param {int} length 长度
     * @param {char} add 补位字符
     */
    static std::string ShiftLeft(const std::string &str, int length, const char add = '0');

    /**
     * @brief 右侧补位
     * @param {string} str 输入字符串
     * @param {int} length 长度
     * @param {char} add 补位字符
     */
    static std::string ShiftRight(const std::string &str, int length, const char add = '0');

    /**
     * @brief 是否为英文
     * @param {string} str 输入字符串
     * @return {bool} 真假
     */
    static bool IsEnglish(const std::string &str);

    /**
     * @brief 是否为中文
     * @param {string} str 输入字符串
     * @return {bool} 真假
     */
    static bool IsChinese(const std::string &str);

    /**
     * @brief 是否为数字
     * @param {string} str 输入字符串
     * @return {bool} 真假
     */
    static bool IsNumeric(const std::string &str);

    /**
     * @brief 转小写
     * @param {string} str 输入字符串
     */
    static std::string ToLower(const std::string &str);

    /**
     * @brief 转大写
     * @param {string} str 输入字符串
     */
    static std::string ToUpper(const std::string &str);

    /**
     * @brief 字符串是否相同
     * @param {string} str1 输入字符串1
     * @param {string} str2 输入字符串2
     * @param {bool} ignoreUpLow 忽略大小写
     * @return {bool} 真假
     */
    static bool IsSame(const std::string &str1, const std::string &str2, bool ignoreUpLow = false);

    /**
     * @brief 获取子串
     * @param {string} str 输入字符串
     * @param {string} start 开始标志
     * @param {string} end 结束标志
     * @param {bool} needEnd 必须匹配结束标志
     * @return {string}
     */
    static std::string Substring(const std::string &str, const std::string &start = "", const std::string &end = "", bool needEnd = false);
};
}

