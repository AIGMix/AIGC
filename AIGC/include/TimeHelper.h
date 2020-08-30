/**********
* Date    :   2020/08/30
* Author  :   Yaronzz
* Contact :   yaronhuang@foxmail.com
* Desc    :   
**********/
#pragma once
#include <string>
#include <chrono>

namespace AIGC
{
    class TimeHelper
    {
    public:
        /**
         * @brief 开始计时
         */
        void Start();

        /**
         * @brief 结束计时(秒)
         * @return {int64_t} 秒
         */
        int64_t EndS();

        /**
         * @brief 结束计时(毫秒)
         * @return {int64_t} 毫秒
         */
        int64_t EndMS();

        /**
         * @brief 获取当前时间戳
         * @return {type}
         */
        static int CurTimeStamp();

        /**
         * @brief 获取当前时间
         * @return {string} 时:分:秒
         */
        static std::string CurTime();

        /**
         * @brief 获取当前日期
         * @return {string} 年/月/日
         */
        static std::string CurDate();

        /**
         * @brief 判断是否为闰年
         * @param {int} iYear 年份
         * @return {bool} 是否
         */
        static bool IsLeapYear(int iYear);

        /**
         * @brief 获取一个月有多少天
         * @param {int} iYear 年份
         * @param {int} iMonth 月
         * @return {int} 天数
         */
        static int GetDayNumOfMonth(int iYear, int iMonth);

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
    };
} 
