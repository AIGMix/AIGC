#include "RandomHelper.h"
#include <iostream>
#include <ctime>

namespace aigc
{
static std::string G_NUMBERS = "0123456789";
static std::string G_SYBOMLS = "!@#$%^&*(){}[]<>,./=-+~|;'\"";
static std::string G_LOWERCAST = "abcdefghijklmnopqrstuvwxyz";
static std::string G_UPPERCAST = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
static bool G_HAVE_INIT = false;

int RandomHelper::GetInt(int min, int max)
{
    if (min >= max)
        return min;

    //设置种子
    if (G_HAVE_INIT == false)
    {
        srand(time(0));
        G_HAVE_INIT = true;
    }

    int ret = rand() % (max - min) + min;
    return ret;
}

std::string RandomHelper::GetString(int length, bool haveNumber, bool haveSymbol, bool uppercase, bool lowercase)
{
    std::string pattern = "";
    if (haveNumber)
        pattern += G_NUMBERS;
    if (haveSymbol)
        pattern += G_SYBOMLS;
    if (uppercase)
        pattern += G_UPPERCAST;
    if (lowercase)
        pattern += G_LOWERCAST;
    
    int patternLen = pattern.length();
    if (patternLen <= 0 || length <= 0)
        return "";
    
    std::string ret = "";
    for (int i = 0; i < length; i++)
    {
        int random = GetInt(0, patternLen - 1);
        ret += pattern[random];
    }
    
    return ret;
}

}


// int test()
// {
//     for (size_t i = 0; i < 10; i++)
//     {
//         std::string random = aigc::RandomHelper::GetString(10);
//         std::cout << random << std::endl;
//     }
//     return 0;
// }
