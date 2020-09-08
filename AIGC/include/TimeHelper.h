#pragma once
#include <string>
#include <chrono>

namespace aigc
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
     * @return 秒
     */
    int64_t EndS() const;

    /**
     * @brief 结束计时(毫秒)
     * @return 毫秒
     */
    int64_t EndMS() const;

    /**
     * @brief 休眠
     * @param msDuration 毫秒
     */
    static void SleepMS(int msDuration);

    /**
     * @brief 获取当前时间戳
     */
    static int64_t CurTimeStamp();

    /**
     * @brief 获取当前时间
     * @param sep 分隔符
     * @return 时:分:秒
     */
    static std::string CurTime(std::string sep = ":");

    /**
     * @brief 获取当前日期
     * @param sep 分隔符
     * @return 年/月/日
     */
    static std::string CurDate(std::string sep = "/");

    /**
     * @brief 判断是否为闰年
     * @param year 年份
     * @return 是否
     */
    static bool IsLeapYear(int year);

    /**
     * @brief 获取一个月有多少天
     * @param year 年份
     * @param month 月
     * @return 天数
     */
    static int GetDayNumOfMonth(int year, int month);

private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
};

} 
