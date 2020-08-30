#include "StringHelper.h"

namespace AIGC
{
    std::string StringHelper::TrimLeft(const std::string &pStr, const std::string pRemove)
    {
        std::string str = pStr;
        if (str.length() <= 0 || pRemove.length() <= 0)
            return str;

        while (true)
		{
            int index = str.find(pRemove);
            if (index != 0)
                return str;

            str = str.substr(pRemove.length());
        }
    }

    std::string StringHelper::TrimRight(const std::string &pStr, const std::string pRemove)
    {
        std::string str = pStr;
        if (pStr.length() <= 0 || pRemove.length() <= 0)
            return str;

        while (true)
		{
            int index = str.find_last_of(pRemove);
            if (index != str.length() - 1)
                return str;

            str = str.substr(0, str.length() - pRemove.length());
        }
	}

    std::string StringHelper::Trim(const std::string &pStr, const std::string pRemove)
    {
        std::string str = pStr;
        str = TrimLeft(str, pRemove);
        str = TrimRight(str, pRemove);
        return str;
    }

    bool StringHelper::IsEnglish(const std::string &pStr)
    {
        int len = pStr.length();
        if (len <= 0)
            return true;

        for (int i = 0; i < len; i++)
        {
            //A-Z 65-90 a-z 97-122
            if ((int)pStr[i] >= 65 && (int)pStr[i] <= 90)
                continue;
            if ((int)pStr[i] >= 97 && (int)pStr[i] <= 122)
                continue;
            return false;
        }
        return true;
    }

    bool StringHelper::IsChinese(const std::string &pStr)
    {
        unsigned char utf[4] = {0};
        unsigned char unicode[3] = {0};
        for (int i = 0; i < pStr.length(); i++)
        {
            if ((pStr[i] & 0x80) == 0)
                return false;
            else 
            {
                utf[0] = pStr[i];
                utf[1] = pStr[i + 1];
                utf[2] = pStr[i + 2];
                i++;
                i++;
                unicode[0] = ((utf[0] & 0x0F) << 4) | ((utf[1] & 0x3C) >> 2);
                unicode[1] = ((utf[1] & 0x03) << 6) | (utf[2] & 0x3F);
                if (unicode[0] >= 0x4e && unicode[0] <= 0x9f)
                {
                    if (unicode[0] == 0x9f && unicode[1] > 0xa5)
                        return false;
                }
                else
                    return false;
            }
        }
        return true;
    }

    bool StringHelper::IsNumeric(const std::string &pStr)
    {
        if (pStr.length() <= 0)
            return true;
        
        for (int i = 0; i < pStr.length(); i++)
        {
            if ((int)pStr[i] < 48 || (int)pStr[i] > 57)
                return false;
        }
        return true;
    }

    std::string StringHelper::ToLower(const std::string &pStr)
    {
        std::string str = pStr;
        for (int i = 0; i < str.length(); i++)
        {
            //A-Z 65-90 a-z 97-122
            if ((int)str[i] >= 65 && (int)str[i] <= 90)
                str[i] = 97 + str[i] - 65;
        }
        return str;
    }

    std::string StringHelper::ToUpper(const std::string &pStr)
    {
        std::string str = pStr;
        for (int i = 0; i < str.length(); i++)
        {
            //A-Z 65-90 a-z 97-122
            if ((int)str[i] >= 97 && (int)str[i] <= 122)
                str[i] = 65 + str[i] - 97;
        }
        return str;
    }

    bool StringHelper::IsSame(const std::string& pStr1, const std::string& pStr2, bool bUpLower)
    {
        if (bUpLower)
            return pStr1 == pStr2 ? true : false;
        return ToLower(pStr1) == ToLower(pStr2) ? true : false;
    }

    std::string StringHelper::Substring(const std::string &pStr, const std::string &pStart, const std::string &pEnd, bool bNeedEnd)
    {
        if (pStr.length() <= 0)
            return "";
        if (pStart.length() <= 0 && pEnd.length() <= 0)
            return "";

        int iStartIdx = pStart.length() > 0 ? pStr.find(pStart) : 0;
        if (iStartIdx < 0)
            return "";
        int iEndIdx = pEnd.length() > 0 ? pStr.find(pEnd, iStartIdx) : 0;

        //只需匹配结尾
        if (pStart.length() <= 0)
        {
            if (iEndIdx < 0)
                return "";
            return pStr.substr(0, iEndIdx);
        }
        //只匹配开头
        if (pEnd.length() <= 0)
        {
            return pStr.substr(iStartIdx + pStart.length());
        }

        //硬性要求前后都匹配
        if (bNeedEnd && iEndIdx < 0)
            return "";
        if (iEndIdx >= 0)
            return pStr.substr(iStartIdx + pStart.length(), iEndIdx - iStartIdx - pStart.length());
        else
            return pStr.substr(iStartIdx + pStart.length());
    }
}
