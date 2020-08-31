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
        int64_t EndS() const;

        /**
         * @brief 结束计时(毫秒)
         * @return {int64_t} 毫秒
         */
        int64_t EndMS() const;

        /**
         * @brief 休眠
         * @param {int} iMSDuration 毫秒
         */
        static void SleepMS(int iMSDuration);

        /**
         * @brief 获取当前时间戳
         * @return {type}
         */
        static int64_t CurTimeStamp();

        /**
         * @brief 获取当前时间
         * @param {string} sSep 分隔符
         * @return {string} 时:分:秒
         */
        static std::string CurTime(std::string sSep = ":");

        /**
         * @brief 获取当前日期
         * @param {string} sSep 分隔符
         * @return {string} 年/月/日
         */
        static std::string CurDate(std::string sSep = "/");

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
