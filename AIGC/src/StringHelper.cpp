#include "StringHelper.h"

namespace aigc
{

bool StringHelper::IsEmpty(const std::string &str)
{
    if (str.length() <= 0)
        return true;
    for (int i = 0; i < (int)str.length(); i++)
    {
        if (str[i] == ' ' || str[i] == '\n')
            continue;
        return false;
    }
    return true;
}

bool StringHelper::IsInclude(const std::string &str, const std::string &substr)
{
    if (str.find(substr) == std::string::npos)
        return false;
    return true;
}

bool StringHelper::IsInclude(const std::string &str, char subchar)
{
    if (str.find(subchar) == std::string::npos)
        return false;
    return true;
}

std::string StringHelper::TrimLeft(const std::string &str, const std::string remove)
{
    std::string newStr = str;
    if (newStr.length() <= 0 || remove.length() <= 0)
        return newStr;

    while (true)
    {
        int index = newStr.find(remove);
        if (index != 0)
            return newStr;

        newStr = newStr.substr(remove.length());
    }
}

std::string StringHelper::TrimRight(const std::string &str, const std::string remove)
{
    std::string newStr = str;
    if (newStr.length() <= 0 || remove.length() <= 0)
        return newStr;

    while (true)
    {
        int index = newStr.find_last_of(remove);
        if (index != newStr.length() - 1)
            return newStr;

        newStr = newStr.substr(0, newStr.length() - remove.length());
    }
}

std::string StringHelper::Trim(const std::string &str, const std::string remove)
{
    std::string newStr = str;
    newStr = TrimLeft(newStr, remove);
    newStr = TrimRight(newStr, remove);
    return newStr;
}

std::string StringHelper::ShiftLeft(const std::string &str, int length, const char add)
{
    int len = length - str.length();
    if (len <= 0)
        return str;

    std::string newStr = "";
    for (int i = 0; i < len; i++)
    {
        newStr += add;
    }

    return newStr + str;
}

std::string StringHelper::ShiftRight(const std::string &str, int length, const char add)
{
    int len = length - str.length();
    if (len <= 0)
        return str;

    std::string newStr = "";
    for (int i = 0; i < len; i++)
    {
        newStr += add;
    }

    return str + newStr;
}

bool StringHelper::IsEnglish(const std::string &str)
{
    int len = str.length();
    if (len <= 0)
        return true;

    for (int i = 0; i < len; i++)
    {
        //A-Z 65-90 a-z 97-122
        if ((int)str[i] >= 65 && (int)str[i] <= 90)
            continue;
        if ((int)str[i] >= 97 && (int)str[i] <= 122)
            continue;
        return false;
    }
    return true;
}

bool StringHelper::IsChinese(const std::string &str)
{
    unsigned char utf[4] = {0};
    unsigned char unicode[3] = {0};
    for (int i = 0; i < (int)str.length(); i++)
    {
        if ((str[i] & 0x80) == 0)
            return false;
        else 
        {
            utf[0] = str[i];
            utf[1] = str[i + 1];
            utf[2] = str[i + 2];
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

bool StringHelper::IsNumeric(const std::string &str)
{
    if (str.length() <= 0)
        return true;
    
    for (int i = 0; i < (int)str.length(); i++)
    {
        if ((int)str[i] < 48 || (int)str[i] > 57)
            return false;
    }
    return true;
}

bool StringHelper::IsIP(const std::string &str)
{
    if (str.length() <= 0)
        return false;

    std::vector<std::string> lines = Split(Trim(str), '.');
    if (lines.size() != 4)
        return false;
    
    std::string flag = "0123456789";
    for (int i = 0; i < lines.size(); i++)
    {
        for (int j = 0; j < lines[i].length(); j++)
        {
            if (!IsInclude(flag, lines[i][j]))
                return false;
        }

        int temp = atoi(lines[i].c_str());
        if (temp < 0 || temp > 255)
            return false;
    }

    return true;
}

std::string StringHelper::ToLower(const std::string &str)
{
    std::string newStr = str;
    for (int i = 0; i < (int)newStr.length(); i++)
    {
        //A-Z 65-90 a-z 97-122
        if ((int)newStr[i] >= 65 && (int)newStr[i] <= 90)
            newStr[i] = 97 + newStr[i] - 65;
    }
    return newStr;
}

std::string StringHelper::ToUpper(const std::string &str)
{
    std::string newStr = str;
    for (int i = 0; i < (int)newStr.length(); i++)
    {
        //A-Z 65-90 a-z 97-122
        if ((int)newStr[i] >= 97 && (int)newStr[i] <= 122)
            newStr[i] = 65 + newStr[i] - 97;
    }
    return newStr;
}

bool StringHelper::IsSame(const std::string& str1, const std::string& str2, bool ignoreUpLow)
{
    if (!ignoreUpLow)
        return str1 == str2 ? true : false;
    return ToLower(str1) == ToLower(str2) ? true : false;
}

std::string StringHelper::Substring(const std::string &str, const std::string &start, const std::string &end, bool needEnd)
{
    if (str.length() <= 0)
        return "";
    if (start.length() <= 0 && end.length() <= 0)
        return "";

    int iStartIdx = start.length() > 0 ? str.find(start) : 0;
    if (iStartIdx < 0)
        return "";
    int iEndIdx = end.length() > 0 ? str.find(end, iStartIdx) : 0;

    //只需匹配结尾
    if (start.length() <= 0)
    {
        if (iEndIdx < 0)
            return "";
        return str.substr(0, iEndIdx);
    }
    //只匹配开头
    if (end.length() <= 0)
    {
        return str.substr(iStartIdx + start.length());
    }

    //硬性要求前后都匹配
    if (needEnd && iEndIdx < 0)
        return "";
    if (iEndIdx >= 0)
        return str.substr(iStartIdx + start.length(), iEndIdx - iStartIdx - start.length());
    else
        return str.substr(iStartIdx + start.length());
}

std::vector<std::string> StringHelper::Split(const std::string &str, char seperator)
{
    std::vector<std::string> ret;
    auto string_find_first_not = [str, seperator](size_t pos = 0) -> size_t {
        for (size_t i = pos; i < str.size(); i++)
        {
            if (str[i] != seperator)
                return i;
        }
        return std::string::npos;
    };

    size_t lastPos = string_find_first_not(0);
    size_t pos = str.find(seperator, lastPos);
    while (lastPos != std::string::npos)
    {
        ret.emplace_back(str.substr(lastPos, pos - lastPos));
        lastPos = string_find_first_not(pos);
        pos = str.find(seperator, lastPos);
    }

    return ret;
}

std::string StringHelper::Replace(const std::string &str, const std::string &fromStr, const std::string &toStr)
{
    std::string ret = str;
    std::string::size_type pos = 0;
    std::string::size_type srclen = fromStr.size();
    std::string::size_type dstlen = toStr.size();

    while ((pos = ret.find(fromStr, pos)) != std::string::npos)
    {
        ret = ret.replace(pos, srclen, toStr);
        pos += dstlen;
    }

    return ret;
}

}


// int main()
// {
//     bool flag = aigc::StringHelper::IsIP("127.0. 0.1");
//     return 0;
// }
