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
     * @param {int} min 最小值
     * @param {int} max 最大值
     * @return {int}
     */
    static int GetInt(int min, int max);

    /**
     * @brief 获取随机字符串
     * @param {int} length 字符串长度
     * @param {bool} haveNumber 是否包括数字
     * @param {bool} haveSymbol 是否包括符号
     * @param {bool} uppercase 是否包括大写字母
     * @param {bool} lowercase 是否包括小写字母
     * @return {string}
     */
    static std::string GetString(int length, bool haveNumber = true, bool haveSymbol = true, bool uppercase = true, bool lowercase = true);

};

}
