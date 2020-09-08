/**********
* Date    :   2020/09/07
* Author  :   Yaronzz
* Contact :   yaronhuang@foxmail.com
* Desc    :   
**********/
#pragma once

#include <string>
#include <vector>

namespace aigc
{

class RandomHelper
{
public:
    /**
     * @brief 获取随机数
     * @param min 最小值
     * @param max 最大值
     */
    static int GetInt(int min, int max);

    /**
     * @brief 获取随机字符串
     * @param length 字符串长度
     * @param haveNumber 是否包括数字
     * @param haveSymbol 是否包括符号
     * @param uppercase 是否包括大写字母
     * @param lowercase 是否包括小写字母
     */
    static std::string GetString(int length, bool haveNumber = true, bool haveSymbol = true, bool uppercase = true, bool lowercase = true);

};

}
