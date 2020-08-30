#include "TimeHelper.h"

namespace AIGC
{
    /// 每个月份已经过去的日子数量
    static int G_MONTH_DAYS[2][13] = {
        {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365},
        {0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335, 366}, //闰年
    };

    void TimeHelper::Start()
    {
        m_Start = std::chrono::high_resolution_clock::now();
    }

    int64_t TimeHelper::EndS()
    {
        return std::chrono::duration_cast<std::chrono::duration<double>>(std::chrono::high_resolution_clock::now() - m_Start).count();
    }

    int64_t TimeHelper::EndMS()
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - m_Start).count();
    }

    bool TimeHelper::IsLeapYear(int iYear)
    {
        //四年一闰，百年不闰，四百年再闰
        if ((iYear % 4 == 0) && (iYear % 100 != 0) || (iYear % 400 == 0))
            return true;
        return false;
    }

    int TimeHelper::GetDayNumOfMonth(int iYear, int iMonth)
    {
        if (iMonth <= 0 || iMonth > 12 || iYear < 0)
            return 0;

        if (IsLeapYear(iYear))
            return G_MONTH_DAYS[1][iMonth] - G_MONTH_DAYS[1][iMonth - 1];
        return G_MONTH_DAYS[0][iMonth] - G_MONTH_DAYS[0][iMonth - 1];
    }
}