#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif // _WIN32

#include <time.h>
#include "StringHelper.h"
#include "TimeHelper.h"

namespace aigc
{
/// 每个月份已经过去的日子数量
static int G_MONTH_DAYS[2][13] = {
    {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
    {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}, //闰年
};

typedef struct AIGCTimeStruct
{
    int Year;                    //年
    int Month;                   //月
    int Day;                     //日
    int DayOfWeek;               //周几
    int Hour;                    //时
    int Minute;                  //分
    int Second;                  //秒
    int Milliseconds;            //毫秒
}AIGCTime;

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
static AIGCTime GetAIGTime()
{
    int64_t stamp = TimeHelper::CurTimeStamp();
    int64_t bjstamp = stamp + 8 * 3600; // 时间先加8小时转换为北京时间
    int64_t lTemp = bjstamp % 86400;    // 对日子(24*3600)求余

    AIGCTime ret;
    ret.Hour = (int)(lTemp / 3600);  // 时
    lTemp = lTemp % 3600;
    ret.Minute = (int)(lTemp / 60);  // 分
    ret.Second = (int)(lTemp % 60);  // 秒

    int64_t lDayCount = bjstamp / 86400; // 从1970.1.1到in_ulUTC的天数
    ret.DayOfWeek = (lDayCount + 4) % 7; // 星期几
    ret.Year = (int)(lDayCount / 365 + 1970);   // 年

    while (1)
    {
        if (CalcUTCDayNum(ret.Year) > lDayCount)
            break;
        ret.Year++;
    }
    ret.Year -= 1;

    lTemp = lDayCount - CalcUTCDayNum(ret.Year);
    int bLeapYear = TimeHelper::IsLeapYear(ret.Year) ? 1 : 0;
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

void TimeHelper::Start()
{
    m_start = std::chrono::high_resolution_clock::now();
}

int64_t TimeHelper::EndS() const
{
    return (int64_t)std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_start).count();
}

int64_t TimeHelper::EndMS() const
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_start).count();
}

void TimeHelper::SleepMS(int msDuration)
{
#ifdef _WIN32
    Sleep((DWORD)msDuration);
#else
    usleep(msDuration * 1000);
#endif // _WIN32
}

int64_t TimeHelper::CurTimeStamp()
{
    time_t aRawTime;
    time(&aRawTime);
    return aRawTime;
}

std::string TimeHelper::CurTime(std::string sep)
{
    AIGCTime time = GetAIGTime();
    std::string hour = StringHelper::ShiftLeft(std::to_string(time.Hour), 2, '0');
    std::string minute = StringHelper::ShiftLeft(std::to_string(time.Minute), 2, '0');
    std::string second = StringHelper::ShiftLeft(std::to_string(time.Second), 2, '0');
    return hour + sep + minute + sep + second;
}

std::string TimeHelper::CurDate(std::string sep)
{
    AIGCTime time = GetAIGTime();
    std::string year = StringHelper::ShiftLeft(std::to_string(time.Year), 4, '0');
    std::string month = StringHelper::ShiftLeft(std::to_string(time.Month), 2, '0');
    std::string day = StringHelper::ShiftLeft(std::to_string(time.Day), 2, '0');
    return year + sep + month + sep + day;
}

std::string TimeHelper::CurDateTime(std::string sep, std::string timeSep)
{
    AIGCTime time = GetAIGTime();
    std::string year = StringHelper::ShiftLeft(std::to_string(time.Year), 4, '0');
    std::string month = StringHelper::ShiftLeft(std::to_string(time.Month), 2, '0');
    std::string day = StringHelper::ShiftLeft(std::to_string(time.Day), 2, '0');
    std::string hour = StringHelper::ShiftLeft(std::to_string(time.Hour), 2, '0');
    std::string minute = StringHelper::ShiftLeft(std::to_string(time.Minute), 2, '0');
    std::string second = StringHelper::ShiftLeft(std::to_string(time.Second), 2, '0');
    return year + sep + month + sep + day + " " + hour + timeSep + minute + timeSep + second;
}

bool TimeHelper::IsLeapYear(int year)
{
    //四年一闰，百年不闰，四百年再闰
    if ((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
        return true;
    return false;
}

int TimeHelper::GetDayNumOfMonth(int year, int month)
{
    if (month <= 0 || month > 12 || year < 0)
        return 0;

    if (IsLeapYear(year))
        return G_MONTH_DAYS[1][month] - G_MONTH_DAYS[1][month - 1];
    return G_MONTH_DAYS[0][month] - G_MONTH_DAYS[0][month - 1];
}

}