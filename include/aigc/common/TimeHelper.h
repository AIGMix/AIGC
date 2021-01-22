#pragma once
#include <string>
#include <chrono>
#include <time.h>

#include "StringHelper.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32


namespace aigc
{

    typedef struct AIGCTimeStruct
    {
        int Year;         //年
        int Month;        //月
        int Day;          //日
        int DayOfWeek;    //周几
        int Hour;         //时
        int Minute;       //分
        int Second;       //秒
        int Milliseconds; //毫秒
    } AIGCTime;

    class TimeHelper
    {
    public:
        /**
         * @brief 开始计时
         */
        void Start()
        {
            m_start = std::chrono::high_resolution_clock::now();
        }

        /**
         * @brief 结束计时(秒)
         * @return 秒
         */
        int64_t EndS() const
        {
            return (int64_t)std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_start).count();
        }

        /**
         * @brief 结束计时(毫秒)
         * @return 毫秒
         */
        int64_t EndMS() const
        {
            return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_start).count();
        }

        /**
         * @brief 休眠
         * @param msDuration 毫秒
         */
        static void SleepMS(int msDuration)
        {
    #ifdef _WIN32
            Sleep((DWORD)msDuration);
    #else
            usleep(msDuration * 1000);
    #endif // _WIN32
        }

        /**
         * @brief 获取当前时间戳
         */
        static int64_t CurTimeStamp()
        {
            time_t aRawTime;
            time(&aRawTime);
            return aRawTime;
        }

        /**
         * @brief 获取当前时间
         * @param sep 分隔符
         * @return 时:分:秒
         */
        static std::string CurTime(std::string sep = ":")
        {
            AIGCTime time = GetAIGCTime();
            std::string hour = StringHelper::ShiftLeft(std::to_string(time.Hour), 2, "0");
            std::string minute = StringHelper::ShiftLeft(std::to_string(time.Minute), 2, "0");
            std::string second = StringHelper::ShiftLeft(std::to_string(time.Second), 2, "0");
            return hour + sep + minute + sep + second;
        }

        /**
         * @brief 获取当前日期
         * @param sep 分隔符
         * @return 年-月-日
         */
        static std::string CurDate(std::string sep = "-")
        {
            AIGCTime time = GetAIGCTime();
            std::string year = StringHelper::ShiftLeft(std::to_string(time.Year), 4, "0");
            std::string month = StringHelper::ShiftLeft(std::to_string(time.Month), 2, "0");
            std::string day = StringHelper::ShiftLeft(std::to_string(time.Day), 2, "0");
            return year + sep + month + sep + day;
        }

        /**
         * @brief 获取当前日期时间
         * @param sep 分隔符
         * @param timeSep 分隔符
         * @return 年-月-日 时:分:秒
         */
        static std::string CurDateTime(std::string sep = "-", std::string timeSep = ":")
        {
            AIGCTime time = GetAIGCTime();
            std::string year = StringHelper::ShiftLeft(std::to_string(time.Year), 4, "0");
            std::string month = StringHelper::ShiftLeft(std::to_string(time.Month), 2, "0");
            std::string day = StringHelper::ShiftLeft(std::to_string(time.Day), 2, "0");
            std::string hour = StringHelper::ShiftLeft(std::to_string(time.Hour), 2, "0");
            std::string minute = StringHelper::ShiftLeft(std::to_string(time.Minute), 2, "0");
            std::string second = StringHelper::ShiftLeft(std::to_string(time.Second), 2, "0");
            return year + sep + month + sep + day + " " + hour + timeSep + minute + timeSep + second;
        }

        /**
         * @brief 判断是否为闰年
         * @param year 年份
         * @return 是否
         */
        static bool IsLeapYear(int year)
        {
            //四年一闰，百年不闰，四百年再闰
            if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
                return true;
            return false;
        }

        /**
         * @brief 获取一个月有多少天
         * @param year 年份
         * @param month 月
         * @return 天数
         */
        static int GetDayNumOfMonth(int year, int month)
        {
            if (month <= 0 || month > 12 || year < 0)
                return 0;

            /// 每个月份已经过去的日子数量
            static int G_MONTH_DAYS[2][13] = {
                {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
                {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}, //闰年
            };
            if (IsLeapYear(year))
                return G_MONTH_DAYS[1][month] - G_MONTH_DAYS[1][month - 1];
            return G_MONTH_DAYS[0][month] - G_MONTH_DAYS[0][month - 1];
        }

        /**
         * @brief 计算从1970年1月1日到 xx年1月1日的天数
         * @param {int} year 年份
         * @return {int} 天数
         */
        static int CalcUTCDayNum(int year)
        {
            year -= 1;
            return year * 365 + (year / 4 - year / 100 + year / 400) - 719162;
        }

        /**
         * @brief 获取当前时间
         * @return {AIGCTime}
         */
        static AIGCTime GetAIGCTime()
        {
            int64_t stamp = TimeHelper::CurTimeStamp();
            int64_t bjstamp = stamp + 8 * 3600; // 时间先加8小时转换为北京时间
            int64_t lTemp = bjstamp % 86400;    // 对日子(24*3600)求余

            AIGCTime ret;
            ret.Hour = (int)(lTemp / 3600); // 时
            lTemp = lTemp % 3600;
            ret.Minute = (int)(lTemp / 60); // 分
            ret.Second = (int)(lTemp % 60); // 秒

            int64_t lDayCount = bjstamp / 86400;      // 从1970.1.1到in_ulUTC的天数
            ret.DayOfWeek = (lDayCount + 4) % 7;      // 星期几
            ret.Year = (int)(lDayCount / 365 + 1970); // 年

            while (1)
            {
                if (CalcUTCDayNum(ret.Year) > lDayCount)
                    break;
                ret.Year++;
            }
            ret.Year -= 1;

            lTemp = lDayCount - CalcUTCDayNum(ret.Year);
            int bLeapYear = TimeHelper::IsLeapYear(ret.Year) ? 1 : 0;

            /// 每个月份已经过去的日子数量
            static int G_MONTH_DAYS[2][13] = {
                {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
                {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}, //闰年
            };
            for (int i = 1; i < 13; i++)
            {
                if (G_MONTH_DAYS[bLeapYear][i] > lTemp)
                {
                    ret.Month = i;
                    ret.Day = (int)(lTemp - G_MONTH_DAYS[bLeapYear][i - 1] + 1);
                    break;
                }
            }
            return ret;
        }

    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> m_start;
    };

} 
