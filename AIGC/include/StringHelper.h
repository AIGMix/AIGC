 #pragma once
#include <string>

namespace AIGC
{
	class StringHelper
	{
	public:
        /**
         * @brief 去除头部字符
         * @param {string} pStr 输入字符串
         * @param {string} pRemove 要去除的字符
         */
        static std::string TrimLeft(const std::string &pStr, const std::string pRemove = " ");

        /**
         * @brief 去除尾部字符
         * @param {string} pStr 输入字符串
         * @param {string} pRemove 要去除的字符
         */
        static std::string TrimRight(const std::string &pStr, const std::string pRemove = " ");

        /**
         * @brief 去除头尾
         * @param {string} pStr 输入字符串
         * @param {string} pRemove 要去除的字符
         */
        static std::string Trim(const std::string &pStr, const std::string pRemove = " ");

        /**
         * @brief 左侧补位
         * @param {string} pStr 输入字符串
         * @param {int} iLength 长度
         * @param {char} pAdd 补位字符
         */
        static std::string ShiftLeft(const std::string &pStr, int iLength, const char pAdd = '0');

        /**
         * @brief 右侧补位
         * @param {string} pStr 输入字符串
         * @param {int} iLength 长度
         * @param {char} pAdd 补位字符
         */
        static std::string ShiftRight(const std::string &pStr, int iLength, const char pAdd = '0');

        /**
         * @brief 是否为英文
         * @param {string} pStr 输入字符串
         * @return {bool} 真假
         */
        static bool IsEnglish(const std::string &pStr);

        /**
         * @brief 是否为中文
         * @param {string} pStr 输入字符串
         * @return {bool} 真假
         */
        static bool IsChinese(const std::string &pStr);

        /**
         * @brief 是否为数字
         * @param {string} pStr 输入字符串
         * @return {bool} 真假
         */
        static bool IsNumeric(const std::string &pStr);

        /**
         * @brief 转小写
         * @param {string} pStr 输入字符串
         */
        static std::string ToLower(const std::string &pStr);

        /**
         * @brief 转大写
         * @param {string} pStr 输入字符串
         */
        static std::string ToUpper(const std::string &pStr);

        /**
         * @brief 字符串是否相同
         * @param {string} pStr1 输入字符串1
         * @param {string} pStr2 输入字符串2
         * @param {bool} bUpLower 是否区分大小写
         * @return {bool} 真假
         */
        static bool IsSame(const std::string& pStr1, const std::string& pStr2, bool bUpLower = true);

        /**
         * @brief 获取子串
         * @param {string} pStr 输入字符串
         * @param {string} pStart 开始标志
         * @param {string} pEnd 结束标志
         * @param {bool} bNeedEnd 必须匹配结束标志
         * @return {string}
         */
        static std::string Substring(const std::string &pStr, const std::string &pStart = "", const std::string &pEnd = "", bool bNeedEnd = false);
    };
}

