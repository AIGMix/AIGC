// AIGC.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>

#include "StringHelper.h"
#include <stdio.h>
#include <chrono>
#include "HttpHelper.h"
#include "JsonHelper.h"
#include "TimeHelper.h"
#include "MathHelper.h"
#include "TidalClient.h"

using namespace std;
using namespace AIGC;

#pragma comment(lib, "ws2_32.lib")

int main()
{
    Point point1;
    point1.dX = 1;
    point1.dY = 3;
    Point point2;
    point2.dX = 2;
    point2.dY = 4;
    LineEquation equ = MathHelper::GetLineEquation(point1, point2);

    string time = TimeHelper::CurTime();
    string date = TimeHelper::CurDate();

    string chinese = "中文字符";
    string english = "english";
    string number = "11035";

    string sub = StringHelper::Substring(english, "n", "1", true);

    string english2 = StringHelper::ToUpper(english);

    bool bflag;
    bflag = StringHelper::IsChinese(chinese);
    bflag = StringHelper::IsEnglish(english);
    bflag = StringHelper::IsNumeric(number);


    std::cout
        << "hello" << std::endl;
    string strJson = "{\"name\":\"MenAngel\",\"age\":23,\"hobbys\":{\"age\":23}}";


    string value = JsonHelper::GetValue(strJson, {"name"}, string(""));

    return 0;
}

